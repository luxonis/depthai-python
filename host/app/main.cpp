#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <exception>
#include <iostream>
#include <vector>
#include <string>

#include "depthai_constants.hpp"
#include "pipeline/host_pipeline.hpp"
#include "program_raw_options.hpp"
#include "stream/stream_info.hpp"
#include "xlink/xlink_wrapper.hpp"
#include "host_logs_writer.hpp"
#include "../core/host_data_reader.hpp"
#include "../core/nnet/tensor_info.hpp"
#include "../core/nnet/tensor_info_helper.hpp"
#include "../core/host_json_helper.hpp"

#define DEPTHAI_EXTRAS_PATH "../../../../"

const char* c_mvcmd_file = DEPTHAI_EXTRAS_PATH "depthai.cmd";
const char* config_file = DEPTHAI_EXTRAS_PATH "resources/pipeline-config/config.json";
std::string device_calibration_file = DEPTHAI_EXTRAS_PATH "resources/calibration/default.calib";

const char* nn_config_fpath = DEPTHAI_EXTRAS_PATH "resources/nn/object_detection_4shave/object_detection.json";
const char* nn_config_validation_schema_fpath = DEPTHAI_EXTRAS_PATH "resources/nn/json_validation_schema.json";


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

int g_break_counter = 0;
bool g_break_app_flag = false;


void breakHandler(int s)
{
    printf("depthai: Caught signal %d. Going to stop the app.\n", s);

    g_break_app_flag = true;

    if (g_break_counter != 0) { exit(1); }
    ++g_break_counter;
}


int main(int argc, char** argv)
try
{
    std::cout << "depthai: Start.\n";
    int return_value = EXIT_FAILURE;

    do
    {
        // handler for Ctrl+C
        struct sigaction sigIntHandler;
        sigIntHandler.sa_handler = breakHandler;
        sigemptyset(&sigIntHandler.sa_mask);
        sigIntHandler.sa_flags = 0;
        sigaction(SIGINT, &sigIntHandler, NULL);


        // cnn options
        // std::string json_fpath = "../../../../depthai-resources/nn/MobileSSD_20classes/mobilenet_ssd.json";
        // std::vector<TensorInfo> tensor_infos;
        // if (!parseTensorInfosFromJsonFile(json_fpath, tensor_infos)) { printf("Can\'t load tensor json configs\n"); }


        // validation json
        json json_valid_obj;
        auto json_valid_fpath = std::string(nn_config_validation_schema_fpath);
        if (!getJSONFromFile(json_valid_fpath, json_valid_obj))
        {
            std::cout << "error: getJSONFromFile (validation)\n";
            break;
        }


        // nn configuration json
        auto json_fpath = std::string(nn_config_fpath);

        json json_obj;
        if (!getJSONFromFile(json_fpath, json_obj, &json_valid_obj))
        {
            std::cout << "error: getJSONFromFile (configuration)\n";
            break;
        }


        // pipeline configurations json
        auto json_config_fpath = std::string(config_file);

        json json_config_obj;
        if (!getJSONFromFile(json_config_fpath, json_config_obj)) // TODO: add validation
        {
            std::cout << "error: getJSONFromFile (configuration)\n";
            break;
        }



        // read program options
        ProgramRawOptions raw_options = readProgramRawOptions(argc, argv);


        // mvcmd boot is required ?
        std::string mvcmd_file = "";
                // raw_options.hasOption("dont_load_mvcmd")
                //     ? ""
                //     : c_mvcmd_file;

        // Myriad configurations
        unsigned char disparity_confidence = std::atoi(raw_options.getValueOrDefault("disparity_confidence", "200"));


        // calibration file -> "config_h2d" : "_board/"
        std::vector<float> homography_buff = {
            // default for BW0250TG:
             9.8806816e-01,  2.9474013e-03,  5.0676174e+00,
            -8.7650679e-03,  9.9214733e-01, -8.7952757e+00,
            -8.4495878e-06, -3.6034894e-06,  1.0000000e+00
        };

        if (device_calibration_file.empty())
        {
            std::cout << "depthai: Calibration file is not specified, will use default setting;\n";
        }
        else
        {
            HostDataReader calibration_reader;
            if (!calibration_reader.init(device_calibration_file))
            {
                std::cout << "depthai: Error opening calibration file: " << device_calibration_file << "\n";
                break;
            }

            int sz = calibration_reader.getSize();
            assert(sz == sizeof(float) * 9);
            std::cout << "Read: " << calibration_reader.readData(reinterpret_cast<unsigned char*>(homography_buff.data()), sz) << std::endl;
        }

        json_config_obj["_board"] =
        {
            {"_homography_right_to_left", homography_buff}
        };


        // xlink connection between PC & Myriad
        XLinkWrapper xlink(/* be_verbose */ true);

        std::cout << "depthai: before xlink init;\n";

        if (!xlink.initFromHostSide(
                    &g_xlink_global_handler,
                    &g_xlink_device_handler,
                    mvcmd_file.c_str(),
                    /* reboot */ true)
            )
        {
            std::cout << "depthai: Error initalizing xlink;\n";
            break;
        }


        // config_h2d
        std::string json_config_str_packed = json_config_obj.dump();
        // resize, as xlink expects exact;y the same size for input:
        assert(json_config_str_packed.size() < g_streams_pc_to_myriad.at("config_h2d").size);
        json_config_str_packed.resize(g_streams_pc_to_myriad.at("config_h2d").size, 0);

        if (!xlink.openWriteAndCloseStream(
                g_streams_pc_to_myriad.at("config_h2d"),
                json_config_str_packed.data())
            )
        {
            std::cout << "depthai: pipelineConfig write error;\n";
            break;
        }


        // --- BLOB FILE ---
        // if (USE_PIPELINE_AI)
        {
            std::string blob_file = "../../../../depthai-resources/nn/MobileSSD_retail/person-detection-retail-0013.blob";

            // read & pass blob file
            if (blob_file.empty())
            {
                std::cout << "depthai: Blob file is not specified, will use default setting;\n";
            }
            else
            {
                HostDataReader _blob_reader;
                if (!_blob_reader.init(blob_file))
                {
                    std::cout << "depthai: Error opening blob file: " << blob_file << "\n";
                    break;
                }
                int size_blob = _blob_reader.getSize();

                std::vector<uint8_t> buff_blob(size_blob);

                std::cout << "Read: " << _blob_reader.readData(buff_blob.data(), size_blob) << std::endl;

                if (!xlink.openWriteAndCloseStream(g_streams_pc_to_myriad.at("inBlob"), buff_blob.data()))
                {
                    std::cout << "depthai: pipelineConfig write error;\n";
                    break;
                }
                printf("depthai: done sending Blob file %s\n", blob_file.c_str());
            }
        }


        // update depth size
        assert(false && "dev app is outdated (read of streams, stream elem size)");
        c_streams_myriad_to_pc.at("depth_raw") = StreamInfo("depth_raw", 720*1280*2, { 720, 1280}, 2);

        std::cout << "SIZE of depth pkg: " << c_streams_myriad_to_pc.at("depth_raw").size << "\n";


        // open Myriad -> PC streams
        HostPipeline pipeline;
        bool error_in_open_streams = false;
        int opened_streams_counter = 0;


        std::vector<std::string> streams;

        streams.push_back("meta_d2h");

        if (json_config_obj.at("-pipeline-depth").get<bool>())
        {
            streams.push_back("left");
            streams.push_back("right");
            streams.push_back("depth");
            // streams.push_back("disparity");
        }

        if (json_config_obj.at("-pipeline-ai").get<bool>())
        {
            streams.push_back("previewout");
            streams.push_back("metaout");
        }


        for (std::string stream_name : streams)
        {
            pipeline.makeStreamPublic(stream_name);

            std::cout << "Trying to open stream: " << stream_name << "\n";
            const auto &stream_info = c_streams_myriad_to_pc.at(stream_name);

            // start thread
            if (xlink.openStreamInThreadAndNotifyObservers(stream_info))
            {
                pipeline.observe(xlink, stream_info);
                ++opened_streams_counter;
            }
            else
            {
                error_in_open_streams = true;
            }
        }

        if (error_in_open_streams)
        {
            xlink.stopThreads();
            break;
        }

        // consumer
        if (opened_streams_counter == 0)
        {
            std::cout << "depthai: There is no connected streams, consumer not started;\n";
            xlink.stopThreads();
        }
        else
        {
            std::thread pipeline_consumer(
                [&pipeline, &xlink]()
                {
                    while (true)
                    {
                        std::list<std::shared_ptr<HostDataPacket>> data = pipeline.getAvailableDataPackets();
                        if (data.size() != 0)
                        {
                            std::cout << "Consumed packets:\n";
                            for (const std::shared_ptr<HostDataPacket> &packet : data)
                            {
                                std::cout << "- " << packet->stream_name << " (sz: " << packet->size() << ")\n";

                                if (packet->stream_name == "metaout")
                                {
                                    // TensorRawData t;
                                    // t.data = packet->getData();
                                    // t.size = packet->size();

                                    // std::vector<TensorRawData> t_vec;
                                    // t_vec.push_back(t);

                                    // TensorsEntryIterator iterator(t_vec, tensor_infos);

                                    // int num = iterator.getEntriesNumber();
                                    // std::cout << "Entries number: " << num << "\n";

                                    // for (unsigned i = 0; i < 3 /* num */; ++i)
                                    // {
                                    //     std::vector<TensorEntry> entry = iterator.getEntry(i);

                                    //     for (const TensorEntry& e : entry)
                                    //     {
                                    //         std::cout << "prop.num: " << e.properties_number << "\n";
                                    //         for (unsigned j = 0; j < e.properties_number; ++j)
                                    //         {
                                    //             std::cout << e.getFloatByIndex(j) << "\n";
                                    //         }

                                    //         std::cout << "id: "         << e.getFloat("id")         << "\n";
                                    //         std::cout << "label: "      << e.getFloat("label")      << "\n";
                                    //         std::cout << "confidence: " << e.getFloat("confidence") << "\n";
                                    //         std::cout << "left: "       << e.getFloat("left")       << "\n";
                                    //         std::cout << "top: "        << e.getFloat("top")        << "\n";
                                    //         std::cout << "right: "      << e.getFloat("right")      << "\n";
                                    //         std::cout << "bottom: "     << e.getFloat("bottom")     << "\n";
                                    //     }
                                    // }
                                }
                            }
                        }

                        if (g_break_app_flag)
                        {
                            xlink.stopThreads();
                            break;
                        }

                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    }
                }
            );
            pipeline_consumer.join();
        }

        return_value = EXIT_SUCCESS;
    }
    while (false);

    std::cout << "depthai: DONE.\n";
    return return_value;
}
catch (const std::exception &e)
{
    std::cout << "depthai: global exception: " << e.what() << "\n";
}
catch (...)
{
    std::cout << "depthai: global exception.\n";
}

