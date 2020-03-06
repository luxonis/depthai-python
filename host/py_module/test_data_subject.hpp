#pragma once

#include <chrono>
#include <memory>
#include <thread>

#include "stream/stream_data.hpp"
#include "stream/stream_info.hpp"
#include "general/data_subject.hpp"


class TestDataSubject
    : public DataSubject<StreamInfo, StreamData>
{
public:
    TestDataSubject() {}

    virtual ~TestDataSubject()
    {
        _thr_exit_flag = true;
        if (nullptr != _thr)
        {
            _thr->join();
        }
    }


    void runInThread(const StreamInfo &stream_info)
    {
        unsigned sz = (stream_info.size == 0) ? 1400 : stream_info.size;
        _data = std::vector<unsigned char>(sz, 0);

        _thr = std::unique_ptr<std::thread>(
            new std::thread(
                &TestDataSubject::threadFunc, this, stream_info)
        );
    }

private:
    void threadFunc(const StreamInfo &stream_info)
    {
        int counter = 0;

        while (!_thr_exit_flag)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            StreamData data;
            data.data = _data.data();
            data.size = _data.size();
            data.packet_number = counter;

            this->notifyObservers(stream_info, data);

            counter += 1;

            std::cout << "TestDataSubject: tick\n";
        }
    }

    std::vector<unsigned char>   _data;
    std::unique_ptr<std::thread> _thr;
    bool                         _thr_exit_flag = false;
};
