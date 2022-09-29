#include "depthai_pybind11_tests.hpp"

#include "depthai/depthai.hpp"

TEST_SUBMODULE(xlink_exceptions, m) {

    m.def("throw_xlink_error", [](){
        throw dai::XLinkError(XLINK_ERROR, "stream", "Yikes!");
    });

    m.def("throw_xlink_read_error", [](){
        throw dai::XLinkReadError(XLINK_ERROR, "stream_read");
    });

    m.def("throw_xlink_write_error", [](){
        throw dai::XLinkWriteError(XLINK_ERROR, "stream_write");
    });

}
