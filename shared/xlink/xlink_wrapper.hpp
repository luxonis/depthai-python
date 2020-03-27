#pragma once

#include <list>
#include <mutex>
#include <string>
// #include <shared_mutex>
#include <thread>
#include <vector>

// #include <boost/thread/shared_mutex.hpp>

#include <XLinkPublicDefines.h>

#include "../stream/stream_info.hpp"
#include "../general/data_observer.hpp"
#include "../general/data_subject.hpp"
#include "../stream/stream_data.hpp"

#include <fcntl.h> 
#define MAX_USB_BUFF (5 * 1024 * 1024)

///
/// XLinkWrapper is class that wraps XLinks communication.
/// This class uses two mutexes, so be careful editing the code.
/// It can be used for both side communication (in/out):
/// 
///
/// For example, to pass configurations to device:
///
///                   (as DataObserver)
///    DataSubject  ->  XLinkWrapper   -> USB
///
///
///  For example, to pass frames from device:
///
///                    (as DataSubject)
///    DataObserver <-  XLinkWrapper   <- USB
///
class XLinkWrapper
    : public DataSubject<StreamInfo, StreamData>
    , public DataObserver<StreamInfo, StreamData>
{
public:
    XLinkWrapper(bool be_verbose);
    virtual ~XLinkWrapper();

#ifdef __PC__
    bool initFromHostSide   (
        XLinkGlobalHandler_t* global_handler,
        XLinkHandler_t* device_handler,
        const std::string &path_to_mvcmd = "",
        const std::string &usb_device = "",
        bool reboot_device_on_destructor = true
        );
#endif // __PC__
#ifndef __PC__
    bool initFromDeviceSide (XLinkGlobalHandler_t* global_handler);
#endif // __PC__

    uint32_t openReadAndCloseStream (const StreamInfo &stream, std::string &stl_container) const;
    uint32_t openReadAndCloseStream (const StreamInfo &stream, void* buffer, uint32_t buffer_size) const;
    bool     openWriteAndCloseStream(const StreamInfo &stream, const void* write_data) const;
    bool     openStreamInThreadAndNotifyObservers(const StreamInfo &stream, bool async_connect = false);

#ifdef __PC__
    static void rebootDevice(int device_id);
#endif // __PC__

    void stopThreads();


private:
    bool     initXLink(XLinkGlobalHandler_t* global_handler) const;
    int      openStream    (const StreamInfo &stream) const;
    bool     writeToStream (const StreamInfo &stream, int stream_id, const void* write_data, uint32_t write_data_size) const;
    void     closeStream   (const std::string &stream_name, int &stream_id) const;

    void openAndReadDataThreadFunc(const StreamInfo &stream, int stream_id);
    void waitForThreads();

    const char* convertErrorCodeToString(XLinkError_t error_code) const;

    void onNewDataSubject(const StreamInfo &stream_info) override;
    void onNewData(const StreamInfo &stream_info, const StreamData &data) override;
    void closeAllObserverStreams();


    const int              c_stream_open_tries = 5;
    const unsigned         c_stream_write_timeout_ms = 5000;
    const unsigned         c_stream_write_wait_ms = 1;
    // const int              c_stream_write_tries = 20;
    const unsigned         c_stream_read_timeout_ms = 500;
    const unsigned         c_stream_read_wait_ms = 1;
    const unsigned         c_stream_read_thread_wait_ms = 1;


    const bool             _be_verbose;
#ifdef __PC__
    bool                   _reboot_device_on_destructor = false;
#endif // __PC__

    int                    _device_link_id = -1; // -1 stands for undefined

    volatile bool                   _threads_exit       = false;
    std::mutex             _threads_subject_list_lock; // used in: openStreamInThreadAndNotifyObservers, waitForThreads
    std::list<std::thread> _threads_subject;

    mutable std::mutex     _open_stream_lock; // used in: openStream

    std::map<std::string, int> _observer_id_to_stream_id; // stream name -> xlink`s stream id
    // boost::shared_mutex        _observer_id_to_stream_id_lock; // read/write lock, info:
    // lock for read: onNewData
    // lock for write: onNewDataSubject, closeAllObserverStreams
};
