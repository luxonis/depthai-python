#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <map>

#include <boost/algorithm/string/replace.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include "../core/device_support_listener.hpp"
#include "../core/host_data_packet.hpp"
#include "../core/host_data_reader.hpp"
#include "../core/nnet/tensor_info.hpp"
#include "../core/nnet/tensor_info_helper.hpp"
#include "../core/nnet/tensor_entry.hpp"
#include "../core/nnet/nnet_packet.hpp"
#include "../core/nnet/tensor_entry_container.hpp"
#include "../core/pipeline/host_pipeline.hpp"
#include "../core/pipeline/host_pipeline_config.hpp"
#include "../core/pipeline/cnn_host_pipeline.hpp"
#include "../core/disparity_stream_post_processor.hpp"
#include "../../shared/cnn_info.hpp"
#include "../../shared/depthai_constants.hpp"
#include "../../shared/json_helper.hpp"
#include "../../shared/version.hpp"
#include "../../shared/xlink/xlink_wrapper.hpp"
#include "../core/host_json_helper.hpp"
#include "host_capture_command.hpp"


namespace py = pybind11;

std::string config_backup;
std::string cmd_backup;
std::string usb_device_backup;
std::shared_ptr<CNNHostPipeline> gl_result = nullptr;

static volatile std::atomic<int> wdog_keep;

bool soft_deinit_device();
bool init_device(
    const std::string &device_cmd_file,
    const std::string &usb_device
);
std::shared_ptr<CNNHostPipeline> create_pipeline(
    const std::string &config_json_str
);

static int wdog_thread_alive = 0;
void wdog_thread(int& wd_timeout_ms)
{
    std::cout << "watchdog started " << wd_timeout_ms << std::endl;
    const int sleep_chunk = 100;
    const int sleep_nr = wd_timeout_ms / sleep_chunk;
    while(wdog_thread_alive)
    {
        wdog_keep = 0;
        for(int i = 0; i < sleep_nr; i++)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_chunk));
            if(wdog_thread_alive == 0)
            {
                break;
            }
        }
        if(wdog_keep == 0 && wdog_thread_alive == 1)
        {
            std::cout << "watchdog triggered " << std::endl;
            soft_deinit_device();
            bool init;
            for(int retry = 0; retry < 1; retry++)
            {
                init = init_device(cmd_backup, usb_device_backup);
                if(init)
                {
                    break;
            }
            }
            if(!init)
            {
                exit(9);
            }
            create_pipeline(config_backup);
        }
    }

}

static std::thread wd_thread;
static int wd_timeout_ms = 3000;
int  wdog_start(void)
{
    static int once = 1;
    if(once)
    {
        wdog_thread_alive = 1;
        wd_thread = std::thread(wdog_thread, std::ref(wd_timeout_ms)); 
        once = 0;
    }
    return 0;
}
int  wdog_stop(void)
{
    if(wdog_thread_alive)
    {
        wdog_thread_alive = 0;
        wd_thread.join();
    }
    return 0;
}

//todo
extern "C" {
void wdog_keepalive(void)
{
    wdog_keep = 1;
}

};

// TODO: REMOVE, IT'S TEMPORARY (for test only)
static XLinkGlobalHandler_t g_xlink_global_handler =
{
    .profEnable = 0,  
    .profilingData = {
        .totalReadTime   = 0.f,
        .totalWriteTime  = 0.f,
        .totalReadBytes  = 0,
        .totalWriteBytes = 0,
        .totalBootCount  = 0,
        .totalBootTime   = 0.f
    },
    .loglevel   = 0,
    .protocol   = USB_VSC
};

XLinkHandler_t g_xlink_device_handler =
{
    .devicePath  = NULL,
    .devicePath2 = NULL,
    .linkId      = 0
};
// TODO: END OF REMOVE


// all global data is deleted with "_cleanup" python object
std::unique_ptr<XLinkWrapper> g_xlink; // TODO: make sync
json g_config_d2h;

std::unique_ptr<DisparityStreamPostProcessor> g_disparity_post_proc;
std::unique_ptr<DeviceSupportListener>        g_device_support_listener;
std::unique_ptr<HostCaptureCommand>           g_host_caputure_command;

std::map<std::string, int> nn_to_depth_mapping = {
    { "off_x", 0 },
    { "off_y", 0 },
    { "max_w", 0 },
    { "max_h", 0 },
};


bool init_device(
    const std::string &device_cmd_file,
    const std::string &usb_device
)
{
    cmd_backup = device_cmd_file;
    usb_device_backup = usb_device;
    bool result = false;
    std::string error_msg;

    do
    {
        // xlink
        if (nullptr != g_xlink)
        {
            error_msg = "Device is already initialized.";
            std::cout << error_msg << "\n";
            break;
        }

        g_xlink = std::unique_ptr<XLinkWrapper>(new XLinkWrapper(true));

        if (!g_xlink->initFromHostSide(
                &g_xlink_global_handler,
                &g_xlink_device_handler,
                device_cmd_file,
                usb_device,
                true)
            )
        {
            std::cout << "depthai: Error initializing xlink\n";
            break;
        }

        wdog_start();

        // config_d2h
        {
            printf("Loading config file\n");

            std::string config_d2h_str;
            StreamInfo si("config_d2h", 102400);

            int config_file_length = g_xlink->openReadAndCloseStream(
                    si,
                    config_d2h_str
                    );
            if(config_file_length == -1)
            {
                break;
            }
            if (!getJSONFromString(config_d2h_str, g_config_d2h))
            {
                std::cout << "depthai: error parsing config_d2h\n";
            }
        }


        // check version
        {
            std::string device_version = g_config_d2h.at("_version").get<std::string>();
            if (device_version != c_depthai_version)
            {
                printf("Version does not match (%s & %s)\n",
                    device_version.c_str(), c_depthai_version);
                break;
            }

            std::string device_dev_version = g_config_d2h.at("_dev_version").get<std::string>();
            if (device_dev_version != c_depthai_dev_version)
            {
                printf("WARNING: Version (dev) does not match (%s & %s)\n",
                    device_dev_version.c_str(), c_depthai_dev_version);
            }
        }

        uint32_t version = g_config_d2h.at("eeprom").at("version").get<int>();
        printf("EEPROM data:");
        if (version == -1) {
            printf(" invalid / unprogrammed\n");
        } else {
            printf(" valid (v%d)\n", version);
            std::string board_name;
            std::string board_rev;
            float rgb_fov_deg = 0;
            bool stereo_center_crop = false;
            if (version >= 2) {
                board_name = g_config_d2h.at("eeprom").at("board_name").get<std::string>();
                board_rev  = g_config_d2h.at("eeprom").at("board_rev").get<std::string>();
                rgb_fov_deg= g_config_d2h.at("eeprom").at("rgb_fov_deg").get<float>();
            }
            if (version >= 3) {
                stereo_center_crop = g_config_d2h.at("eeprom").at("stereo_center_crop").get<bool>();
            }
            float left_fov_deg = g_config_d2h.at("eeprom").at("left_fov_deg").get<float>();
            float left_to_right_distance_m = g_config_d2h.at("eeprom").at("left_to_right_distance_m").get<float>();
            float left_to_rgb_distance_m = g_config_d2h.at("eeprom").at("left_to_rgb_distance_m").get<float>();
            bool swap_left_and_right_cameras = g_config_d2h.at("eeprom").at("swap_left_and_right_cameras").get<bool>();
            std::vector<float> calib = g_config_d2h.at("eeprom").at("calib").get<std::vector<float>>();
            printf("  Board name     : %s\n", board_name.empty() ? "<NOT-SET>" : board_name.c_str());
            printf("  Board rev      : %s\n", board_rev.empty()  ? "<NOT-SET>" : board_rev.c_str());
            printf("  HFOV L/R       : %g deg\n", left_fov_deg);
            printf("  HFOV RGB       : %g deg\n", rgb_fov_deg);
            printf("  L-R   distance : %g cm\n", 100 * left_to_right_distance_m);
            printf("  L-RGB distance : %g cm\n", 100 * left_to_rgb_distance_m);
            printf("  L/R swapped    : %s\n", swap_left_and_right_cameras ? "yes" : "no");
            printf("  L/R crop region: %s\n", stereo_center_crop ? "center" : "top");
            printf("  Calibration homography:\n");
            for (int i = 0; i < 9; i++) {
                printf(" %11.6f,", calib.at(i));
                if (i % 3 == 2) printf("\n");
            }
        }




        result = true;
    } while (false);

    if (!result)
    {
        g_xlink = nullptr;
        // TODO: add custom python exception for passing messages
        // throw std::exception();
    }

    return result;
}

bool soft_deinit_device()
{
    g_xlink = nullptr;
    g_disparity_post_proc = nullptr;
    g_device_support_listener = nullptr;
	g_host_caputure_command = nullptr;
    return true;
}

bool deinit_device()
{
    wdog_stop();       
    g_xlink = nullptr;
    g_disparity_post_proc = nullptr;
    g_device_support_listener = nullptr;
    g_host_caputure_command = nullptr;
	gl_result = nullptr;
    return true;
}

std::vector<std::string> get_available_steams()
{
    std::vector<std::string> result;

    if (g_config_d2h.is_object() &&
        g_config_d2h.contains("_available_streams") &&
        g_config_d2h.at("_available_streams").is_array()
        )
    {
        for (const auto &obj : g_config_d2h.at("_available_streams"))
        {
            result.push_back(obj.get<std::string>());
        }
    }

    return result;
}

void request_jpeg(){
    if(g_host_caputure_command != nullptr){
        g_host_caputure_command->capture();
    }
}

std::map<std::string, int> get_nn_to_depth_bbox_mapping()
{
    return nn_to_depth_mapping;
}

std::shared_ptr<CNNHostPipeline> create_pipeline(
    const std::string &config_json_str
)
{
    config_backup = config_json_str;

    bool init_ok = false;
    do
    {
        // check xlink
        if (nullptr == g_xlink)
        {
            std::cout << "device is not initialized\n";
            break;
        }

        // str -> json
        json config_json;
        if (!getJSONFromString(config_json_str, config_json))
        {
            std::cout << "Error: Cant parse json config :" << config_json_str << "\n";
            break;
        }

        // json -> configurations
        HostPipelineConfig config;
        if (!config.initWithJSON(config_json))
        {
            std::cout << "Error: Cant init configs with json: " << config_json.dump() << "\n";
            break;
        }

        // read tensor info
        std::vector<TensorInfo>       tensors_info;
        if (parseTensorInfosFromJsonFile(config.ai.blob_file_config, tensors_info))
        {
            std::cout << "CNN configurations read: " << config.ai.blob_file_config.c_str() << "\n";
        }
        else
        {
            std::cout << "There is no cnn configuration file or error in it\'s parsing: " << config.ai.blob_file_config.c_str() << "\n";
        }


        // pipeline configurations json
        // homography
        std::vector<float> homography_buff = {
            // default for BW0250TG:
             9.8806816e-01,  2.9474013e-03,  5.0676174e+00,
            -8.7650679e-03,  9.9214733e-01, -8.7952757e+00,
            -8.4495878e-06, -3.6034894e-06,  1.0000000e+00
        };
        bool stereo_center_crop = false;

        if (config.depth.calibration_file.empty())
        {
            std::cout << "depthai: Calibration file is not specified, will use default setting;\n";
        }
        else
        {
            HostDataReader calibration_reader;
            if (!calibration_reader.init(config.depth.calibration_file))
            {
                std::cout << "depthai: Error opening calibration file: " << config.depth.calibration_file << "\n";
                break;
            }

            const int homography_size = sizeof(float) * 9;
            int sz = calibration_reader.getSize();
            assert(sz >= homography_size);
            calibration_reader.readData(reinterpret_cast<unsigned char*>(homography_buff.data()), homography_size);
            int flags_size = sz - homography_size;
            if (flags_size > 0)
            {
                assert(flags_size == 1);
                calibration_reader.readData(reinterpret_cast<unsigned char*>(&stereo_center_crop), 1);
            }
        }

        json json_config_obj;

        // Add video configuration if specified
        if(config_json.count("video_config") > 0){
            json_config_obj["video_config"] = config_json["video_config"]; 
        }

        json_config_obj["board"]["clear-eeprom"] = config.board_config.clear_eeprom;
        json_config_obj["board"]["store-to-eeprom"] = config.board_config.store_to_eeprom;
        json_config_obj["board"]["override-eeprom"] = config.board_config.override_eeprom;
        json_config_obj["board"]["swap-left-and-right-cameras"] = config.board_config.swap_left_and_right_cameras;
        json_config_obj["board"]["left_fov_deg"] = config.board_config.left_fov_deg;
        json_config_obj["board"]["rgb_fov_deg"] = config.board_config.rgb_fov_deg;
        json_config_obj["board"]["left_to_right_distance_m"] = config.board_config.left_to_right_distance_m;
        json_config_obj["board"]["left_to_rgb_distance_m"] = config.board_config.left_to_rgb_distance_m;
        json_config_obj["board"]["stereo_center_crop"] = config.board_config.stereo_center_crop || stereo_center_crop;
        json_config_obj["board"]["name"] = config.board_config.name;
        json_config_obj["board"]["revision"] = config.board_config.revision;
        json_config_obj["_board"] =
        {
            {"_homography_right_to_left", homography_buff}
        };
        json_config_obj["depth"]["padding_factor"] = config.depth.padding_factor;
        json_config_obj["depth"]["depth_limit_mm"] = (int)(config.depth.depth_limit_m * 1000);
        json_config_obj["depth"]["confidence_threshold"] = config.depth.confidence_threshold;

        json_config_obj["_load_inBlob"] = true;
        json_config_obj["_pipeline"] =
        {
            {"_streams", json::array()}
        };

        json_config_obj["ai"]["calc_dist_to_bb"] = config.ai.calc_dist_to_bb;
        json_config_obj["ai"]["keep_aspect_ratio"] = config.ai.keep_aspect_ratio;

        bool add_disparity_post_processing_color = false;
        bool temp_measurement = false;

        std::vector<std::string> pipeline_device_streams;

        for (const auto &stream : config.streams)
        {
            if (stream.name == "depth_color_h")
            {
                add_disparity_post_processing_color = true;
                json obj = { {"name", "disparity"} };
                if (0.f != stream.max_fps)     { obj["max_fps"]   = stream.max_fps;   };
                json_config_obj["_pipeline"]["_streams"].push_back(obj);
            }
            else
            {
                if (stream.name == "meta_d2h")
                {
                    temp_measurement = true;
                }
                json obj = { {"name" ,stream.name} };

                if (!stream.data_type.empty()) { obj["data_type"] = stream.data_type; };
                if (0.f != stream.max_fps)     { obj["max_fps"]   = stream.max_fps;   };

                // TODO: temporary solution
                if (stream.name == "depth_sipp")
                        // {
                        //     obj["data_type"] = "uint8";
                        //     c_streams_myriad_to_pc["depth_sipp"] = StreamInfo("depth_sipp",     0, { 720, 1280}  );
                        // }
                        {
                            obj["data_type"] = "uint16";
                            c_streams_myriad_to_pc["depth_sipp"] = StreamInfo("depth_sipp",     0, { 720, 1280}, 2  );
                        }
                        // {
                        //     obj["data_type"] = "rgb";
                        //     c_streams_myriad_to_pc["depth_sipp"] = StreamInfo("depth_sipp",     2764800, { 720, 1280, 3} );
                        // }

                json_config_obj["_pipeline"]["_streams"].push_back(obj);
                pipeline_device_streams.push_back(stream.name);
            }
        }


        // host -> "config_h2d" -> device
        std::string pipeline_config_str_packed = json_config_obj.dump();
        std::cout << "config_h2d json:\n" << pipeline_config_str_packed << "\n";
        // resize, as xlink expects exact;y the same size for input:
        assert(pipeline_config_str_packed.size() < g_streams_pc_to_myriad.at("config_h2d").size);
        pipeline_config_str_packed.resize(g_streams_pc_to_myriad.at("config_h2d").size, 0);

        if (!g_xlink->openWriteAndCloseStream(
                g_streams_pc_to_myriad.at("config_h2d"),
                pipeline_config_str_packed.data())
            )
        {
            std::cout << "depthai: pipelineConfig write error;\n";
            break;
        }

        // host -> "host_capture" -> device
        auto stream = g_streams_pc_to_myriad.at("host_capture");
        g_host_caputure_command = std::unique_ptr<HostCaptureCommand>(new HostCaptureCommand((stream)));
        g_xlink->observe(*g_host_caputure_command, stream);


        // read & pass blob file
        if (config.ai.blob_file.empty())
        {
            std::cout << "depthai: Blob file is not specified, will use default setting;\n";
        }
        else
        {
            HostDataReader _blob_reader;
            if (!_blob_reader.init(config.ai.blob_file))
            {
                std::cout << "depthai: Error opening blob file: " << config.ai.blob_file << "\n";
                break;
            }
            int size_blob = _blob_reader.getSize();

            std::vector<uint8_t> buff_blob(size_blob);

            std::cout << "Read: " << _blob_reader.readData(buff_blob.data(), size_blob) << std::endl;

            // inBlob
            StreamInfo blobInfo;
            blobInfo.name = "inBlob";
            blobInfo.size = size_blob;

            if (!g_xlink->openWriteAndCloseStream(blobInfo, buff_blob.data()))
            {
                std::cout << "depthai: pipelineConfig write error;\n";
                break;
            }
            printf("depthai: done sending Blob file %s\n", config.ai.blob_file.c_str());

            // outBlob
            StreamInfo outBlob;
            outBlob.name = "outBlob";
            //TODO: remove asserts considering StreamInfo size
            outBlob.size = 1;

            cnn_info cnn_input_info;

            static char cnn_info_arr[sizeof(cnn_info)];
            g_xlink->openReadAndCloseStream(
                outBlob,
                (void*)cnn_info_arr,
                sizeof(cnn_info)
                );

            memcpy(&cnn_input_info, &cnn_info_arr, sizeof(cnn_input_info));

            printf("CNN input width: %d\n", cnn_input_info.cnn_input_width);
            printf("CNN input height: %d\n", cnn_input_info.cnn_input_height);
            printf("CNN input num channels: %d\n", cnn_input_info.cnn_input_num_channels);
            printf("CNN to depth bounding-box mapping: start(%d, %d), max_size(%d, %d)\n",
                    cnn_input_info.nn_to_depth.offset_x,
                    cnn_input_info.nn_to_depth.offset_y,
                    cnn_input_info.nn_to_depth.max_width,
                    cnn_input_info.nn_to_depth.max_height);
            nn_to_depth_mapping["off_x"] = cnn_input_info.nn_to_depth.offset_x;
            nn_to_depth_mapping["off_y"] = cnn_input_info.nn_to_depth.offset_y;
            nn_to_depth_mapping["max_w"] = cnn_input_info.nn_to_depth.max_width;
            nn_to_depth_mapping["max_h"] = cnn_input_info.nn_to_depth.max_height;

            // update tensor infos
            assert(!(tensors_info.size() > (sizeof(cnn_input_info.offsets)/sizeof(cnn_input_info.offsets[0]))));

            for (int i = 0; i < tensors_info.size(); i++)
            {
                tensors_info[i].nnet_input_width  = cnn_input_info.cnn_input_width;
                tensors_info[i].nnet_input_height = cnn_input_info.cnn_input_height;
                tensors_info[i].offset = cnn_input_info.offsets[i];
            }

            c_streams_myriad_to_pc["previewout"].dimensions = {
                                                               cnn_input_info.cnn_input_num_channels,
                                                               cnn_input_info.cnn_input_height,
                                                               cnn_input_info.cnn_input_width
                                                               };

            // check CMX slices & used shaves
            int device_cmx_for_nnet = g_config_d2h.at("_resources").at("cmx").at("for_nnet").get<int>();
            if (cnn_input_info.number_of_cmx_slices != device_cmx_for_nnet)
            {
                std::cout << "Error: Blob is compiled for " << cnn_input_info.number_of_cmx_slices
                          << " cmx slices but device can calculate on " << device_cmx_for_nnet << "\n";
                break;
            }

            int device_shaves_for_nnet = g_config_d2h.at("_resources").at("shaves").at("for_nnet").get<int>();
            if (cnn_input_info.number_of_shaves != device_shaves_for_nnet)
            {
                std::cout << "Error: Blob is compiled for " << cnn_input_info.number_of_shaves
                          << " shaves but device can calculate on " << device_shaves_for_nnet << "\n";
                break;
            }
        }


        // sort streams by device specified order
        {
            // mapping: stream name -> array index
            std::vector<std::string> available_streams_ordered = get_available_steams();
            std::unordered_map<std::string, int> stream_name_to_idx;
            for (int i = 0; i < available_streams_ordered.size(); ++i)
            {
                stream_name_to_idx[ available_streams_ordered[i] ] = i;
            }

            // check requested streams are in available streams
            bool wrong_stream_name = false;
            for (const auto &stream_name : pipeline_device_streams)
            {
                if (stream_name_to_idx.find(stream_name) == stream_name_to_idx.end())
                {
                    std::cout << "Error: device does not provide stream: " << stream_name << "\n";
                    wrong_stream_name = true;
                }
            }

            if (wrong_stream_name)
            {
                break;
            }

            // sort
            std::sort(std::begin(pipeline_device_streams), std::end(pipeline_device_streams),
                [&stream_name_to_idx]
                (const std::string &a, const std::string &b)
                {
                    return stream_name_to_idx[a] < stream_name_to_idx[b];
                }
            );
        }


        // pipeline
        if(gl_result == nullptr)
            gl_result = std::shared_ptr<CNNHostPipeline>(new CNNHostPipeline(tensors_info));

        for (const std::string &stream_name : pipeline_device_streams)
        {
            std::cout << "Host stream start:" << stream_name << "\n";

            if (g_xlink->openStreamInThreadAndNotifyObservers(c_streams_myriad_to_pc.at(stream_name)))
            {
                gl_result->makeStreamPublic(stream_name);
                gl_result->observe(*g_xlink.get(), c_streams_myriad_to_pc.at(stream_name));
            }
            else
            {
                std::cout << "depthai: " << stream_name << " error;\n";
                // TODO: rollback correctly!
                break;
            }
        }


        // disparity post processor
        if (add_disparity_post_processing_color)
        {
            g_disparity_post_proc = std::unique_ptr<DisparityStreamPostProcessor>(
                new DisparityStreamPostProcessor(
                    add_disparity_post_processing_color));

            const std::string stream_in_name = "disparity";
            const std::string stream_out_color_name = "depth_color_h";

            if (g_xlink->openStreamInThreadAndNotifyObservers(c_streams_myriad_to_pc.at(stream_in_name)))
            {
                g_disparity_post_proc->observe(*g_xlink.get(), c_streams_myriad_to_pc.at(stream_in_name));

                if (add_disparity_post_processing_color)
                {
                    gl_result->makeStreamPublic(stream_out_color_name);
                    gl_result->observe(*g_disparity_post_proc.get(), c_streams_myriad_to_pc.at(stream_out_color_name));
                }
            }
            else
            {
                std::cout << "depthai: stream open error " << stream_in_name << " (2)\n";
                // TODO: rollback correctly!
                break;
            }
        }

        if(temp_measurement)
        {
            // device support listener
            g_device_support_listener = std::unique_ptr<DeviceSupportListener>(new DeviceSupportListener);

            g_device_support_listener->observe(
                *g_xlink.get(),
                c_streams_myriad_to_pc.at("meta_d2h")
                );
        }

        init_ok = true;
        std::cout << "depthai: INIT OK!\n";
    }
    while (false);

    if (!init_ok)
    {
        gl_result = nullptr;
    }

    return gl_result;
}


PYBIND11_MAKE_OPAQUE(std::list<std::shared_ptr<HostDataPacket>>);
PYBIND11_MAKE_OPAQUE(std::list<std::shared_ptr<NNetPacket>>);


PYBIND11_MODULE(depthai, m)
{
    // TODO: test ownership in python

    std::string _version = c_depthai_version;
    m.attr("__version__") = _version;

    std::string _dev_version = c_depthai_dev_version;
    m.attr("__dev_version__") = _dev_version;

    // init device
    std::string device_cmd_file = "./depthai.cmd";
    std::string usb_device = "";
    m.def(
        "init_device",
        &init_device,
        "Function that establishes the connection with device and gets configurations from it.",
        py::arg("cmd_file") = device_cmd_file,
        py::arg("usb_device") = usb_device
        );
    
    m.def(
        "deinit_device",
        &deinit_device,
        "Function that destroys the connection with device."
        );

    // reboot
    m.def(
        "reboot_device",
        [](int device_id){
            printf("Rebooting device: ...\n");
            XLinkWrapper::rebootDevice(device_id);
            printf("Rebooting device: DONE.\n");
        },
        py::arg("device_id") = 0
        );

    // available streams
    m.def(
        "get_available_steams",
        &get_available_steams,
        "Returns available streams, that possible to retreive from the device."
        );

    m.def(
        "get_nn_to_depth_bbox_mapping",
        &get_nn_to_depth_bbox_mapping,
        "Returns NN bounding-box to depth mapping as a dict of coords: off_x, off_y, max_w, max_h."
        );

    // cnn pipeline
    m.def(
        "create_pipeline",
        [](py::dict config)
        {
            // str(dict) for string representation uses ['] , but JSON requires ["]
            // fast & dirty solution:
            std::string str = py::str(config);
            boost::replace_all(str, "\'", "\"");
            boost::replace_all(str, "None", "null");
            boost::replace_all(str, "True", "true");
            boost::replace_all(str, "False", "false");
            // TODO: make better json serialization

            return create_pipeline(str);
        },
        "Function for pipeline creation",
        py::arg("config") = py::dict()
        );

    
    // depthai.request_jpeg()
    m.def(
        "request_jpeg",
        &request_jpeg,
        "Function to request a still JPEG encoded image ('jpeg' stream must be enabled)"
    );


    // FrameMetadata struct binding
    py::class_<FrameMetadata>(m, "FrameMetadata")
        .def(py::init<>())
        .def("getTimestamp", &FrameMetadata::getTimestamp)
        .def("getFrameType", &FrameMetadata::getFrameType)
        .def("getFrameWidth", &FrameMetadata::getFrameWidth)
        .def("getFrameHeight", &FrameMetadata::getFrameHeight)
        .def("getFrameBytesPP", &FrameMetadata::getFrameBytesPP)
        .def("getStride", &FrameMetadata::getStride)
        .def("getCategory", &FrameMetadata::getCategory)
        .def("getInstanceNum", &FrameMetadata::getInstanceNum)
        .def("getSequenceNum", &FrameMetadata::getSequenceNum)
        ;

    // for PACKET in data_packets:
    py::class_<HostDataPacket, std::shared_ptr<HostDataPacket>>(m, "DataPacket")
        .def_readonly("stream_name", &HostDataPacket::stream_name)
        .def("size", &HostDataPacket::size)
        .def("getData", &HostDataPacket::getPythonNumpyArray, py::return_value_policy::take_ownership)
        .def("getDataAsStr", &HostDataPacket::getDataAsString, py::return_value_policy::take_ownership)
        .def("getMetadata", &HostDataPacket::getMetadata)
        ;

    // nnet_packets, DATA_PACKETS = p.get_available_nnet_and_data_packets()
    py::class_<std::list<std::shared_ptr<HostDataPacket>>>(m, "DataPacketList")
        .def(py::init<>())
        .def("__len__",  [](const std::list<std::shared_ptr<HostDataPacket>> &v) { return v.size(); })
        .def("__iter__", [](std::list<std::shared_ptr<HostDataPacket>> &v)
        {
            return py::make_iterator(v.begin(), v.end());
        }, py::keep_alive<0, 1>()) /* Keep list alive while iterator is used */
        ;


    // NNET_PACKETS, data_packets = p.get_available_nnet_and_data_packets()
    py::class_<std::list<std::shared_ptr<NNetPacket>>>(m, "NNetPacketList")
        .def(py::init<>())
        .def("__len__",  [](const std::list<std::shared_ptr<NNetPacket>> &v) { return v.size(); })
        .def("__iter__", [](std::list<std::shared_ptr<NNetPacket>> &v)
        {
            return py::make_iterator(v.begin(), v.end());
        }, py::keep_alive<0, 1>()) /* Keep list alive while iterator is used */
        ;

    // for NNET_PACKET in nnet_packets:
    py::class_<NNetPacket, std::shared_ptr<NNetPacket>>(m, "NNetPacket")
        .def("get_tensor", &NNetPacket::getTensor, py::return_value_policy::copy)
        .def("get_tensor", &NNetPacket::getTensorByName, py::return_value_policy::copy)
        .def("entries", &NNetPacket::getTensorEntryContainer, py::return_value_policy::copy)
        ;

    // for te in nnet_packet.ENTRIES()
    py::class_<TensorEntryContainer, std::shared_ptr<TensorEntryContainer>>(m, "TensorEntryContainer")
        .def("__len__", &TensorEntryContainer::size)
        .def("__getitem__", &TensorEntryContainer::getByIndex)
        .def("__getitem__", &TensorEntryContainer::getByName)
        .def("__iter__", [](py::object s) { return PyTensorEntryContainerIterator(s.cast<TensorEntryContainer &>(), s); })
        ;

    // for e in nnet_packet.entries():
    //     e <--- (type(e) == list)
    py::class_<PyTensorEntryContainerIterator>(m, "PyTensorEntryContainerIterator")
        .def("__iter__", [](PyTensorEntryContainerIterator &it) -> PyTensorEntryContainerIterator& { return it; })
        .def("__next__", &PyTensorEntryContainerIterator::next)
        ;

    // for e in nnet_packet.entries():
    //     e[0] <--
    py::class_<TensorEntry, std::shared_ptr<TensorEntry>>(m, "TensorEntry")
        .def("__len__", &TensorEntry::getPropertiesNumber)
        .def("__getitem__", &TensorEntry::getFloat)
        .def("__getitem__", &TensorEntry::getFloatByIndex)
        ;


    // while True:
    //     nnet_packets, data_packets = p.get_available_nnet_and_data_packets()
    //     # nnet_packets: depthai.NNetPacketList
    //     # data_packets: depthai.DataPacketList

    //     for t in nnet_packets.getTensors():
    //         pass

    //     for nnet_packet in nnet_packets:
    //         # nnet_packet: depthai.NNetPacket
    //         # nnet_packet.entries(): depthai.TensorEntryContainer

    //         for e in nnet_packet.entries():
    //             # e: list
    //             # e[0]: depthai.TensorEntry
    //             # e[0][0]: float


    py::class_<HostPipeline>(m, "Pipeline")
        .def("get_available_data_packets", &HostPipeline::getAvailableDataPackets, py::return_value_policy::copy)
        ;

    py::class_<CNNHostPipeline, std::shared_ptr<CNNHostPipeline>>(m, "CNNPipeline")
        .def("get_available_data_packets", &CNNHostPipeline::getAvailableDataPackets, py::return_value_policy::copy)
        .def("get_available_nnet_and_data_packets", &CNNHostPipeline::getAvailableNNetAndDataPackets, py::return_value_policy::copy)
        ;


    // module destructor
    auto cleanup_callback = []() {
        deinit_device();
    };

    m.add_object("_cleanup", py::capsule(cleanup_callback));
}
