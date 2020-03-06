#pragma once

#include <assert.h>

#include <algorithm>
#include <map>
#include <vector>

#include <boost/bind.hpp>
#include <boost/signals2/signal.hpp>


template<class DataInfo, class DataContainer>
class DataSubject
{
private:
    using SignalType =          boost::signals2::signal<void (const DataInfo&, const DataContainer&)>;
public:
    using SlotType   = typename boost::signals2::signal<void (const DataInfo&, const DataContainer&)>::slot_type;


    virtual ~DataSubject() {}

    boost::signals2::connection connect(
        const DataInfo &data_info,
        const SlotType &subscriber
    )
    {
        const std::string &data_name = data_info.getName();
        // assert(_signal_map.find(data_name) == _signal_map.end());
        return _signal_map[data_name].connect(subscriber);
    }

protected:
    void notifyObservers(
        const DataInfo &data_info,
        const DataContainer &data
    )
    {
        const std::string &data_name = data_info.getName();
        assert(_signal_map.find(data_name) != _signal_map.end());
        _signal_map[data_name](data_info, data);
    }


private:
    std::map<std::string, SignalType>    _signal_map;
};
