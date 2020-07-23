#pragma once

#include <assert.h>

#include <memory>
#include <unordered_map>
#include <vector>

#include "tensor_entry.hpp"
#include "tensor_info.hpp"
#include "tensor_entry_container.hpp"
#include "../host_data_packet.hpp"


class NNetPacket
{
public:
    NNetPacket(
              std::vector<std::shared_ptr<HostDataPacket>> &tensors_raw_data,
        const std::vector<TensorInfo>                      &tensors_info
    )
        : _tensors_raw_data(tensors_raw_data)
        , _tensors_info(&tensors_info)
        , _tensor_entry_container(new TensorEntryContainer(
                tensors_raw_data, tensors_info))
    {
        for (size_t i = 0; i < tensors_info.size(); ++i)
        {
            _tensor_name_to_index[ tensors_info.at(i).output_tensor_name ] = i;
        }

        if (_tensor_name_to_index.size() != tensors_info.size())
        {
            printf("There are duplication in tensor names!\n");
        }
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

    py::object getMetadata() {
        // TODO
        return _tensors_raw_data[0]->getMetadata();
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
