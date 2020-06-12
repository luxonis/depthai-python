#pragma once

#if (_MSC_VER >= 1910) || !defined(_MSC_VER) 
    #define HAVE_SNPRINTF
#endif

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

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
    #if __cplusplus >= 201703L
        #include <optional>
        using std::optional;
    #elif __cplusplus >= 201402L
        #include <experimental/optional>
        using std::experimental::optional;
    #else
        #error "Undetected optional binding"
    #endif
#endif
