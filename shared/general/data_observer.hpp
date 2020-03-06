#pragma once

#include <map>
#include <string>

#include <boost/bind.hpp>
#include <boost/signals2/signal.hpp>

#include "data_subject.hpp"


template<class DataInfo, class DataContainer>
class DataObserver
{
public:
    virtual ~DataObserver()
    {
        for (auto &it : _connections)
        {
            it.second.disconnect();
        }
        _connections.clear();
    }

    void observe(
        DataSubject<DataInfo, DataContainer>& subject,
        const DataInfo &data_info
    )
    {
        const std::string &data_name = data_info.getName();
        _connections[data_name] = subject.connect(
            data_info,
            boost::bind(&DataObserver::onNewData, this, _1, _2)
            );

        onNewDataSubject(data_info);
    }

protected:
    virtual void onNewDataSubject(const DataInfo &data_info) { (void)(data_info); }
    virtual void onNewData(const DataInfo &data_info, const DataContainer &data) = 0;

private:
    std::map<std::string, boost::signals2::connection>  _connections;
};
