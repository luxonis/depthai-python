#include "py_tensor_entry_container_iterator.hpp"

std::vector<TensorEntry> PyTensorEntryContainerIterator::next()
{
    while(true)
    {
        if (index == seq.size())
        {
            throw pybind11::stop_iteration();
        }
        std::vector<TensorEntry> next_entry = seq.getByIndex(index++);
        if(next_entry.empty())
        {
            continue;
        }
        
        return next_entry;
    }
}
