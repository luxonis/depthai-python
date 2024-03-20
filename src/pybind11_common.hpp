#pragma once

#if (_MSC_VER >= 1910) || !defined(_MSC_VER)
    #define HAVE_SNPRINTF
#endif

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11_json/pybind11_json.hpp>
#include <cstdint>
#include <stack>
#include <depthai/utility/Path.hpp>
#include <utility/SpanBindings.hpp>

// Include docstring file
#include "docstring.hpp"


#include "tl/optional.hpp"
//using tl::optional;
namespace pybind11 { namespace detail {
    template <typename T>
    struct type_caster<tl::optional<T>> : optional_caster<tl::optional<T>> {};
}}


namespace pybind11 { namespace detail {
    template <> struct type_caster<dai::Path> {
    public:

        // This macro establishes the name 'dai::Path' in
        // function signatures and declares a local variable
        // 'value' of type dai::Path
        PYBIND11_TYPE_CASTER(dai::Path, _("Path"));

        // Conversion part 1 (Python->C++): convert a PyObject into a dai::Path
        // instance or return false upon failure. The second argument
        // indicates whether implicit conversions should be applied.
        bool load(handle src, bool) {
            try {
                // Make sure only strings and paths are cast to dai::Path
                // Instead of anything that can be represented as string
                bool isPath = false;
                try{
                    isPath = isinstance(src, module::import("pathlib").attr("PurePath"));
                } catch (const std::exception&) {
                    //ignore
                }
                if(!isinstance<str>(src) && !isPath) {
                    return false;
                }
                str pystr = static_cast<str>(src);
                value = dai::Path(static_cast<std::string>(pystr));
                return true;
            } catch (...) {
                return false;
            }
        }

        // Conversion part 2 (C++ -> Python): convert an dai::Path instance into
        // a Python (string) object. The second and third arguments are used to
        // indicate the return value policy and parent object (for
        // ``return_value_policy::reference_internal``) and are generally
        // ignored by implicit casters.
        static handle cast(dai::Path src, return_value_policy /* policy */, handle /* parent */) {
            return str{src.u8string()};
        }
    };
}} // namespace pybind11::detail


namespace py = pybind11;

using StackFunction = void (*)(pybind11::module &m, void *pCallstack);
using Callstack = std::stack<StackFunction>;

