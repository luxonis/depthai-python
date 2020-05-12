#include "py_tensor_entry_container_iterator.hpp"

std::vector<TensorEntry> PyTensorEntryContainerIterator::next()
{
    if (index == seq.size())
    {
        throw pybind11::stop_iteration();
    }

    return seq.getByIndex(index++);
}
