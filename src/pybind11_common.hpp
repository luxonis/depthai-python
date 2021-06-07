#pragma once

#if (_MSC_VER >= 1910) || !defined(_MSC_VER)
    #define HAVE_SNPRINTF
#endif

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <cstdint>

// Include docstring file
#include "docstring.hpp"


#include "tl/optional.hpp"
//using tl::optional;
namespace pybind11 { namespace detail {
    template <typename T>
    struct type_caster<tl::optional<T>> : optional_caster<tl::optional<T>> {};
}}

namespace py = pybind11;


