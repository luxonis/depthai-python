#pragma once

#include "nnet/tensor_entry_container.hpp"
#include <vector>
#include <pybind11/pybind11.h>

struct PyTensorEntryContainerIterator
{
    PyTensorEntryContainerIterator(
        TensorEntryContainer &seq,
        pybind11::object ref
    )
        : seq(seq)
        , ref(ref)
    {}

    std::vector<TensorEntry> next();

    TensorEntryContainer &seq;
    pybind11::object ref;
    size_t index = 0;
};