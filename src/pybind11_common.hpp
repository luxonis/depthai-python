#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// Recognize boost::optional as valid data type in pybind11
#define USE_BOOST
#ifdef USE_BOOST
#include <boost/optional.hpp>
//using boost::optional;

namespace pybind11 { namespace detail {
    template <typename T>
    struct type_caster<boost::optional<T>> : optional_caster<boost::optional<T>> {};
}}
#else
#include <experimental/optional>
using std::experimental::optional;
#endif
