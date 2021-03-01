#pragma once

// std
#include <vector>

// pybind11
#include "pybind11_common.hpp"

//project
#include "depthai/nnet/tensor_entry_container.hpp"

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