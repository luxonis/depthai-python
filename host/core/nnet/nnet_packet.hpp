#pragma once

#include <assert.h>

#include <memory>
#include <unordered_map>
#include <vector>

#include "tensor_entry.hpp"
#include "tensor_info.hpp"
#include "tensor_entry_container.hpp"
#include "../host_data_packet.hpp"
#include "../pipeline/depth_calculation_interface.hpp"


class NNetPacket
{
public:
    NNetPacket(
              std::vector<std::shared_ptr<HostDataPacket>> &tensors_raw_data,
        const std::vector<TensorInfo>                      &tensors_info,
              DepthCalculationInterface*                    depth_calculation_interface
    )
        : _tensors_raw_data(tensors_raw_data)
        , _tensors_info(&tensors_info)
        , _tensor_entry_container(new TensorEntryContainer(
                tensors_raw_data, tensors_info, depth_calculation_interface))
    {
        assert(nullptr != depth_calculation_interface);

        for (size_t i = 0; i < tensors_info.size(); ++i)
        {
            _tensor_name_to_index[ tensors_info.at(i).output_tensor_name ] = i;
        }

        if (_tensor_name_to_index.size() != tensors_info.size())
        {
            printf("There are duplication in tensor names!\n");
        }
    }

    std::vector<std::shared_ptr<HostDataPacket>> getTensors()
    {
        return _tensors_raw_data;
    }

    unsigned getTensorsNumber() const
    {
        return _tensors_raw_data.size();
    }

#ifdef HOST_PYTHON_MODULE
    py::array* getTensor(unsigned index)
    {
        return _tensors_raw_data[index]->getPythonNumpyArray();
    }

    py::array* getTensorByName(const std::string &name)
    {
        auto it = _tensor_name_to_index.find(name);
        if (it == _tensor_name_to_index.end())
        {
            return nullptr;
        }
        else
        {
            return getTensor(it->second);
        }
    }
#endif

    std::shared_ptr<TensorEntryContainer> getTensorEntryContainer()
    {
        return _tensor_entry_container;
    }

private:
    std::shared_ptr<TensorEntryContainer>              _tensor_entry_container;

          std::vector<std::shared_ptr<HostDataPacket>> _tensors_raw_data;
    const std::vector<TensorInfo>*                     _tensors_info                = nullptr;

    std::unordered_map<std::string, unsigned> _tensor_name_to_index;
};
