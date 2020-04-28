#include <chrono>
#include <vector>
#include <thread>

#include <iostream>

#include <XLink.h>

#include "../shared/stream/stream_info.hpp"
#include "../../shared/timer.hpp"
#include "xlink_wrapper.hpp"

// FIXME use some header
extern "C" void wdog_keepalive(void);


XLinkWrapper::XLinkWrapper(
    bool be_verbose
)
    : _be_verbose(be_verbose)
{}


XLinkWrapper::~XLinkWrapper()
{
    if (_device_link_id != -1)
    {
        if (_be_verbose) { printf("Stopping threads: ...\n"); }
        stopThreads();
        if (_be_verbose) { printf("Stopping threads: DONE.\n"); }

        if (_be_verbose) { printf("Closing all observer streams: ...\n"); }
        closeAllObserverStreams();
        if (_be_verbose) { printf("Closing all observer streams: DONE.\n"); }

#ifdef __PC__
        if (_reboot_device_on_destructor)
        {
            XLinkWrapper::rebootDevice(_device_link_id);
        }
#endif
    }
}


#ifdef __PC__
bool XLinkWrapper::initFromHostSide(
    XLinkGlobalHandler_t* global_handler,
    XLinkHandler_t* device_handler,
    const std::string &device_cmd_file,
    const std::string &usb_device,
    bool reboot_device_on_destructor
)
{
    _reboot_device_on_destructor = reboot_device_on_destructor;

    assert(_device_link_id == -1);

    bool result = false;
    do
    {
        XLinkError_t rc;
        deviceDesc_t deviceDesc = {};
        deviceDesc_t in_deviceDesc = {};
        // Timeouts in seconds
        double timeout_discover = 10;
        double timeout_bootup   = 10;
        double timeout_connect  = 10;
        std::chrono::steady_clock::time_point tstart;
        std::chrono::duration<double> tdiff;

        // TODO: attempt to connect and reset device if found in booted state
        // printf("rebooting ...\n");
        // XLinkResetRemote(0);
        // printf("done. 5 sec. delay\n");
        // std::this_thread::sleep_for(std::chrono::seconds(5));

        if (!initXLink(global_handler)) {
            printf("Failed to initialize xlink!\n");
            break;
        }

        if (usb_device == "list") {
            const int MAX_DEVICES = 32;
            unsigned int numdev = 0;
            deviceDesc_t deviceDescAll[MAX_DEVICES] = {};
            rc = XLinkFindAllSuitableDevices(X_LINK_ANY_STATE, in_deviceDesc,
                    deviceDescAll, MAX_DEVICES, &numdev);
            printf("Detected %d device(s):\n", numdev);
            for (int i = 0; i < numdev; i++) {
                char *port = strdup(deviceDescAll[i].name);
                strtok(port, "-");
                printf("  %-12s on USB port: %s\n", deviceDescAll[i].name, port);
                free(port);
            }
            exit(0);
        }

        if (!usb_device.empty())
            snprintf(in_deviceDesc.name, sizeof in_deviceDesc.name,
                    "%s-ma2480", usb_device.c_str());

        // boot device
        if (!device_cmd_file.empty()) {
            // Find un-booted device
            bool print_found = false;
            tstart = std::chrono::steady_clock::now();
            do {
                rc = XLinkFindFirstSuitableDevice(X_LINK_UNBOOTED, in_deviceDesc, &deviceDesc);
                tdiff = std::chrono::steady_clock::now() - tstart;
                if (rc != X_LINK_SUCCESS) {
                    print_found = true;
                    printf("\rNo USB device [03e7:2485], still looking");
                    if (!usb_device.empty())
                        printf(" on port %s", usb_device.c_str());
                    printf("... %.3fs ", tdiff.count());
                    fflush(stdout);
                } else {
                    if (print_found)
                        printf("[FOUND]\n");
                    break;
                }
            } while (tdiff.count() < timeout_discover);

            if (rc != X_LINK_SUCCESS) {
                printf("NOT FOUND, err code %d\n", rc);
                break;
            }

            printf("Sending device firmware \"cmd_file\": %s\n", device_cmd_file.c_str());
            rc = XLinkBoot(&deviceDesc, device_cmd_file.c_str());
            if (rc != X_LINK_SUCCESS) {
                printf("Failed to boot the device: %s, err code %d\n", deviceDesc.name, rc);
                break;
            }
        } else {
            // Development option, the firmware is loaded via JTAG
            printf("Device boot is skipped. (\"cmd_file\" NOT SPECIFIED !)\n");
        }

        if (!usb_device.empty())
            snprintf(in_deviceDesc.name, sizeof in_deviceDesc.name,
                    "%s-", usb_device.c_str());

        // Search for booted device
        tstart = std::chrono::steady_clock::now();
        do {
            rc = XLinkFindFirstSuitableDevice(X_LINK_BOOTED, in_deviceDesc, &deviceDesc);
            if (rc == X_LINK_SUCCESS)
                break;
            tdiff = std::chrono::steady_clock::now() - tstart;
        } while (tdiff.count() < timeout_bootup);

        if (rc != X_LINK_SUCCESS) {
            printf("Failed to find booted device after boot, err code %d\n", rc);
            break;
        }

        device_handler->devicePath = deviceDesc.name;
        device_handler->protocol = deviceDesc.protocol;

        // Try to connect to device
        tstart = std::chrono::steady_clock::now();
        do {
            rc = XLinkConnect(device_handler);
            if (rc == X_LINK_SUCCESS)
                break;
            tdiff = std::chrono::steady_clock::now() - tstart;
        } while (tdiff.count() < timeout_connect);

        if (rc != X_LINK_SUCCESS) {
            printf("Failed to connect to device, err code %d\n", rc);
            break;
        }

        printf("Successfully connected to device.\n");

        _device_link_id = device_handler->linkId;

        result = true;
    }
    while (false);

    return result;
}
#endif // __PC__


#ifndef __PC__
bool XLinkWrapper::initFromDeviceSide(
    XLinkGlobalHandler_t* global_handler
)
{
    (void)(global_handler);
    assert(_device_link_id == -1);

    if (_be_verbose) { printf("Initializing XLink on device ...\n"); }

    if(initXLink(global_handler))
    {
        if (_be_verbose) { printf("Successfully initialized Xlink!\n"); }
        _device_link_id = 0;
        if (_be_verbose) { printf("Device link set to 0.\n"); }
        return true;
    }
    else
    {
        printf ("Failed to initialize xlink!\n");
        _device_link_id = -1;
        return false;
    }

    return true;
}
#endif // __PC__


uint32_t XLinkWrapper::openReadAndCloseStream(
    const StreamInfo &stream,
    std::string &stl_container
) const
{
    assert(_device_link_id != -1);
    assert(stream.size != 0);

    uint32_t result = 0;
    do
    {
        // open
        int stream_id = openStream(stream);

        if (stream_id == int(INVALID_STREAM_ID))
        {
            printf("Stream not opened: %s\n", stream.name);
            result = -1;
            break;
        }

// TODO: unite code: reading from stream
        streamPacketDesc_t * packet = nullptr;
        XLinkError_t status = XLinkReadData(stream_id, &packet);

        if (X_LINK_SUCCESS == status)
        {
            stl_container.resize(packet->length);
            memcpy(&stl_container[0], packet->data, packet->length);

            result = packet->length;
            wdog_keepalive();
            // release data
            status = XLinkReleaseData(stream_id);
            if (status != X_LINK_SUCCESS)
            {
                printf("XLink release data failed: %x (%s)\n", status, convertErrorCodeToString(status));
            }
        }
        else
        {
            printf("Device get data failed: %x\n", status);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(c_stream_read_thread_wait_ms));


        // close
        closeStream(stream.getName(), stream_id);
    }
    while (false);

    return result;
}


uint32_t XLinkWrapper::openReadAndCloseStream(
    const StreamInfo &stream,
    void* buffer,
    uint32_t buffer_size
) const
{
    assert(_device_link_id != -1);
    assert(stream.size != 0);

    uint32_t result = 0;
    do
    {
        // open
        int stream_id = openStream(stream);

        if (stream_id == int(INVALID_STREAM_ID))
        {
            printf("Stream not opened: %s\n", stream.name);
            break;
        }




// TODO: unite code: reading from stream
        streamPacketDesc_t * packet = nullptr;
        XLinkError_t status = XLinkReadData(stream_id, &packet);

        if (X_LINK_SUCCESS == status)
        {
            uint32_t copy_sz = std::min(buffer_size, packet->length);
            memcpy(buffer, packet->data, copy_sz);
            result = copy_sz;
            wdog_keepalive();
            // release data
            status = XLinkReleaseData(stream_id);
            if (status != X_LINK_SUCCESS)
            {
                printf("XLink release data failed: %x (%s)\n", status, convertErrorCodeToString(status));
            }
        }
        else
        {
            printf("Device get data failed: %x\n", status);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(c_stream_read_thread_wait_ms));





        // close
        closeStream(stream.getName(), stream_id);
    }
    while (false);

    return result;
}


bool XLinkWrapper::openWriteAndCloseStream(
    const StreamInfo &stream,
    const void* write_data
) const
{
    assert(_device_link_id != -1);
    assert(stream.size != 0);

    bool result = false;
    do
    {
        // open
        int stream_id = openStream(stream);

        if (stream_id == int(INVALID_STREAM_ID))
        {
            printf("Stream not opened: %s\n", stream.name);
            break;
        }

        // write
        result = writeToStream(stream, stream_id, write_data, stream.size);

        // close
        closeStream(stream.getName(), stream_id);
    }
    while (false);

    return result;    
}


bool XLinkWrapper::openStreamInThreadAndNotifyObservers(
    const StreamInfo &stream,
    bool async_connect
)
{
    assert(_device_link_id != -1);

    bool result = false;

    do
    {
        std::lock_guard<std::mutex> guard(_threads_subject_list_lock);

        int stream_id = int(INVALID_STREAM_ID);

        if (!async_connect)
        {
            // open
            if (_be_verbose) { printf("Opening stream for read: %s\n", stream.name); }

            stream_id = openStream(stream);
            if (stream_id == int(INVALID_STREAM_ID))
            {
                printf("Stream not opened: %s\n", stream.name);
                return false;
            }
        }

        if (_be_verbose) { printf("Starting thread for stream: %s\n", stream.name); }

        _threads_subject.push_back(std::thread(
            &XLinkWrapper::openAndReadDataThreadFunc,
            this,
            stream,
            stream_id
            ));

        result = true;
    }
    while (false);

    return true;
}

void XLinkWrapper::stopThreads()
{
    _threads_exit = true;
    waitForThreads();
}

#ifdef __PC__
void XLinkWrapper::rebootDevice(
    int device_id
)
{
    // if (_be_verbose)
    { printf("Reseting device: %d.\n", device_id); }
    XLinkResetRemote(device_id);
    // if (_be_verbose)
    { printf("Reseting: DONE.\n"); }
}
#endif // __PC__

bool XLinkWrapper::initXLink(
    XLinkGlobalHandler_t* global_handler
) const
{
    auto status = XLinkInitialize(global_handler);
    if (X_LINK_SUCCESS == status)
    {
        if (_be_verbose) { printf("XLink initialized.\n"); }
        return true;
    }
    else
    {
        printf("Error in XLinkInitialize: %d (%s)\n", status, convertErrorCodeToString(status));
        return false;
    }
}


// TODO: check if SIZE of StreamInfo is non-zero for write stream
int XLinkWrapper::openStream(
    const StreamInfo &stream
) const
{
    assert(strlen(stream.name) != 0 );

    std::unique_lock<std::mutex> lock(_open_stream_lock);
    int stream_id = INVALID_STREAM_ID;

    // Open stream, obtain and fill stream ids
    if (_be_verbose) { printf("Attempting to open stream %s\n", stream.name); }

    if (_device_link_id != -1)
    {
        for (int retry_count = 0;
                (c_stream_open_tries == -1 || retry_count < c_stream_open_tries) && !_threads_exit;
                ++retry_count
                )
        {
            stream_id = XLinkOpenStream(
                _device_link_id,
                stream.name,
                stream.size
                );
            if(stream_id == int(INVALID_STREAM_ID))
            {
                printf("Failed to open stream %s ! Retrying ...\n", stream.name);
            }
            else
            {
                if (_be_verbose) { printf("Successfully opened stream %s with ID #%d!\n", stream.name, stream_id); }
                break;
            }

            /// Give some time before retrying (to not spam screen)
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
    else
    {
        printf("XLink not initialized. No value for device_handler->linkId.");
    }

    return stream_id;
}

bool XLinkWrapper::writeToStream(
    const StreamInfo &stream,
    int stream_id,
    const void* write_data,
    uint32_t write_data_size
) const
{
    if (_be_verbose) { printf("Writing %d bytes to %s\n", int(write_data_size), stream.name); }

    XLinkError_t status;

    // TODO: It's better to use write with time out. Test & uncomment it.
    // int retry_count = 0;
    // do
    // {
    //     status = XLinkWriteDataWithTimeout(
    //         stream_id,
    //         (const uint8_t*)write_data,
    //         write_data_size,
    //         c_stream_write_timeout_ms
    //         );

    //     if (status == X_LINK_TIMEOUT)
    //     {
    //         printf("Write timeout: %s; Retry ...\n", stream.name);
    //         std::this_thread::sleep_for(std::chrono::milliseconds(c_stream_write_wait_ms));
    //     }
    //     else
    //     {
    //         break;
    //     }
    // }
    // while (true);
    // //while(++retry_count < c_stream_write_tries || -1 == c_stream_write_tries);

    status = XLinkWriteData(stream_id, (const uint8_t*)write_data, write_data_size);

    if (status != X_LINK_SUCCESS)
    {
        printf("XLink write data error: %d (%s)\n", status, convertErrorCodeToString(status));
    }
    else
    {
#if defined(__PC__) || 0 // Set to 0 if too verbose on device...
        printf("!!! XLink write successful: %s (%d)\n", stream.name, int(write_data_size));
#endif

    wdog_keepalive();

    }

    return status == X_LINK_SUCCESS;
}


void XLinkWrapper::closeStream(
    const std::string &stream_name,
    int &stream_id
) const
{
    assert(int(INVALID_STREAM_ID) != stream_id);

    if (_be_verbose) { printf("Closing stream %s: ...\n", stream_name.c_str()); }
    XLinkCloseStream(stream_id);
    if (_be_verbose) { printf("Closing stream %s: DONE.\n", stream_name.c_str()); }

    stream_id = INVALID_STREAM_ID;
}


void XLinkWrapper::openAndReadDataThreadFunc(
    const StreamInfo &stream_info,
    int stream_id
)
{
    XLinkError_t status;
    streamPacketDesc_t * packet = nullptr;
    unsigned packet_counter = 0;


    // is stream opened ?
    // it might be opened before thread creation. (for sync xlink streams connectivity)
    if (stream_id == int(INVALID_STREAM_ID))
    {
        // open
        if (_be_verbose) { printf("Opening stream for read: %s\n", stream_info.name); }

        stream_id = openStream(stream_info);
    }


    if (stream_id == int(INVALID_STREAM_ID))
    {
        printf("Stream not opened: %s\n", stream_info.name);
    }
    else
    {
        if (_be_verbose) { printf("Started thread for stream: %s\n", stream_info.name); }

        // main cycle
        while (!_threads_exit)
        {
// #ifdef __PC__
//             // read
//             do
//             {
//                 status = XLinkReadDataWithTimeOut(
//                     stream_id,
//                     &packet,
//                     c_stream_read_timeout_ms
//                     );

//                 if (status == X_LINK_TIMEOUT)
//                 {
//                     printf("Read timeout: %s (%s); Retry ...\n", stream_info.name, convertErrorCodeToString(status));
//                     std::this_thread::sleep_for(std::chrono::milliseconds(c_stream_read_wait_ms));
//                 }
//                 else
//                 {
//                     break;
//                 }
//             }
//             while (true);
// #endif // __PC__
// #ifndef __PC__
            uint32_t xlink_read_us = 0;
            {
                Timer xlink_t;
                status = XLinkReadData(stream_id, &packet);
                xlink_read_us = xlink_t.ellapsed_us();
            }

// #endif  // __PC__

            if (X_LINK_SUCCESS == status)
            {
                Timer t;

                // notify
                StreamData data;
                data.data = packet->data;
                data.size = packet->length;
                data.packet_number = packet_counter;
                data.duration_xlink_read_us = xlink_read_us;

                // if (_be_verbose)
                // {
                //     printf ("Stream id #%d | Name %10s | Packet size: %8u | No.: %4u\n",
                //             stream_id, stream_info.name, packet->length, packet_counter);
                // }
                wdog_keepalive();
                notifyObservers(stream_info, data);

                packet_counter += 1;

                // release data
                status = XLinkReleaseData(stream_id);
                if (status != X_LINK_SUCCESS)
                {
                    printf("XLink release data failed: %x (%s)\n", status, convertErrorCodeToString(status));
                }

                auto notify_dur = t.ellapsed_us();
                // std::cout << "===> c++ XLinkWrapper::openAndReadDataThreadFunc (with notify obs)" << notify_dur << " us\n";
                // std::cout << "===> c++ XLinkWrapper::openAndReadDataThreadFunc (xlink read)" << xlink_read_us << " us\n";
                // std::cout << "===> c++ " << xlink_read_us << " us " << stream_info.name << " sz: " << data.size << "\n";
            }
            else
            {
                printf("Device get data failed: %x\n", status);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(c_stream_read_thread_wait_ms));
        }

        closeStream(stream_info.getName(), stream_id);
    }

    if (_be_verbose) { printf("Thread for %s finished.\n", stream_info.name); }
}


void XLinkWrapper::waitForThreads()
{
    std::lock_guard<std::mutex> guard(_threads_subject_list_lock);

    for (auto &thr : _threads_subject)
    {
        thr.join();
    }

    _threads_subject.clear();
}


const char* XLinkWrapper::convertErrorCodeToString(
    XLinkError_t error_code
) const
{
    // return XLinkErrorToStr(error_code);

    switch(error_code)
    {
        case X_LINK_SUCCESS:                     return "X_LINK_SUCCESS";
        case X_LINK_ALREADY_OPEN:                return "X_LINK_ALREADY_OPEN";
        case X_LINK_COMMUNICATION_NOT_OPEN:      return "X_LINK_COMMUNICATION_NOT_OPEN";
        case X_LINK_COMMUNICATION_FAIL:          return "X_LINK_COMMUNICATION_FAIL";
        case X_LINK_COMMUNICATION_UNKNOWN_ERROR: return "X_LINK_COMMUNICATION_UNKNOWN_ERROR";
        case X_LINK_DEVICE_NOT_FOUND:            return "X_LINK_DEVICE_NOT_FOUND";
        case X_LINK_TIMEOUT:                     return "X_LINK_TIMEOUT";
        case X_LINK_ERROR:                       return "X_LINK_ERROR";
        case X_LINK_OUT_OF_MEMORY:               return "X_LINK_OUT_OF_MEMORY";
        default:                                 return "<UNKNOWN ERROR>";
    }
}


void XLinkWrapper::onNewDataSubject(
    const StreamInfo &stream_info
)
{
    // TODO: use sync. Not safe!

//    std::lock_guard<std::shared_timed_mutex> write_lock(_observer_id_to_stream_id_lock);

    // boost::upgrade_lock< boost::shared_mutex > lock(_observer_id_to_stream_id_lock);
    // boost::upgrade_to_unique_lock<boost::shared_mutex> write_lock(lock);

    if (_be_verbose) { printf("Creating observer stream %s: ...\n", stream_info.name); }

    auto stream_name = stream_info.getName();
    assert(_observer_id_to_stream_id.find(stream_name) == _observer_id_to_stream_id.end()); // There is no such entry

    int stream_id = openStream(stream_info);
    _observer_id_to_stream_id[stream_name] = stream_id;

    if (_be_verbose) { printf("Creating observer stream %s: DONE.\n", stream_info.name); }
}


void XLinkWrapper::onNewData(
    const StreamInfo &stream_info,
    const StreamData &data
)
{
    // TODO: use sync. Not safe!

//    std::shared_lock<std::shared_timed_mutex> read_lock(_observer_id_to_stream_id_lock);

    // boost::shared_lock< boost::shared_mutex > read_lock(_observer_id_to_stream_id_lock);

    do
    {
        if (_be_verbose) { printf("=== New data in observer stream %s, size: %d\n", stream_info.name, int(data.size)); }

        auto stream_name = stream_info.getName();
        assert(_observer_id_to_stream_id.find(stream_name) != _observer_id_to_stream_id.end());

        int stream_id = _observer_id_to_stream_id.at(stream_name);
        if (stream_id == int(INVALID_STREAM_ID))
        {
            printf("Cant write, stream is invalid (not opened): %s.\n", stream_info.name);
            break;
        }

        if (!writeToStream(stream_info, stream_id, data.data, data.size))
        {
            printf("Write error to stream: %s.\n", stream_info.name);
            break;
        }
    }
    while (false);
}

void XLinkWrapper::closeAllObserverStreams()
{
    // TODO: use sync. Not safe!

    // std::lock_guard<std::shared_timed_mutex> write_lock(_observer_id_to_stream_id_lock);

    // boost::upgrade_lock< boost::shared_mutex > lock(_observer_id_to_stream_id_lock);
    // boost::upgrade_to_unique_lock<boost::shared_mutex> write_lock(lock);

    for (auto &it : _observer_id_to_stream_id)
    {
        closeStream(it.first, it.second);
    }

    _observer_id_to_stream_id.clear();
}
