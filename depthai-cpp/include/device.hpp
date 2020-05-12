#pragma once

//std
#include <string>
#include <thread>

//shared
#include "depthai-shared/xlink/xlink_wrapper.hpp"

//project
#include "nlohmann/json.hpp"
#include "pipeline/cnn_host_pipeline.hpp"
#include "pipeline/host_pipeline.hpp"
#include "disparity_stream_post_processor.hpp"
#include "device_support_listener.hpp"


/*
#include "device_support_listener.hpp"
#include "host_data_packet.hpp"
#include "host_data_reader.hpp"
#include "nnet/tensor_info.hpp"
#include "nnet/tensor_info_helper.hpp"
#include "nnet/tensor_entry.hpp"
#include "nnet/nnet_packet.hpp"
#include "nnet/tensor_entry_container.hpp"
#include "pipeline/host_pipeline.hpp"
#include "pipeline/host_pipeline_config.hpp"
#include "pipeline/cnn_host_pipeline.hpp"
#include "disparity_stream_post_processor.hpp"
#include "depthai-shared/cnn_info.hpp"
#include "depthai-shared/depthai_constants.hpp"
#include "depthai-shared/json_helper.hpp"
#include "depthai-shared/xlink/xlink_wrapper.hpp"
*/




// RAII for specific Device device
class Device{

public:

    Device();

    Device(std::string usb_device, bool usb2_mode = false);

    // Basically init_device but RAII
    Device(std::string cmd_file, std::string usb_device);

    // Basically deinit_device but RAII
    ~Device();

    std::shared_ptr<CNNHostPipeline> create_pipeline(
        const std::string &config_json_str
    );
    std::vector<std::string> get_available_streams();


private:
    
    std::vector<uint8_t> patched_cmd;

    void wdog_thread(int& wd_timeout_ms);
    int wdog_start(void);
    int wdog_stop(void);

    bool init_device(
        const std::string &device_cmd_file,
        const std::string &usb_device,
        uint8_t* binary = nullptr,
        long binary_size = 0
    );
    void deinit_device(){
        g_xlink = nullptr;
        g_disparity_post_proc = nullptr;
        g_device_support_listener = nullptr;
    }

    std::shared_ptr<CNNHostPipeline> gl_result = nullptr;


    std::string config_backup;
    std::string cmd_backup;
    std::string usb_device_backup;
    uint8_t* binary_backup;
    long binary_size_backup;

    volatile std::atomic<int> wdog_keep;
    int wdog_thread_alive = 1;

    std::thread wd_thread;
    int wd_timeout_ms = 1000;

    std::unique_ptr<XLinkWrapper> g_xlink; // TODO: make sync
    nlohmann::json g_config_d2h;

    std::unique_ptr<DisparityStreamPostProcessor> g_disparity_post_proc;
    std::unique_ptr<DeviceSupportListener>        g_device_support_listener;

    XLinkHandler_t g_xlink_device_handler =
    {
        .devicePath  = NULL,
        .devicePath2 = NULL,
        .linkId      = 0
    };



};