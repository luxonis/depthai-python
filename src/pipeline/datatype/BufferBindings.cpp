#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/Buffer.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_buffer(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawBuffer, std::shared_ptr<RawBuffer>> rawBuffer(m, "RawBuffer", DOC(dai, RawBuffer));
    py::class_<Buffer, ADatatype, std::shared_ptr<Buffer>> buffer(m, "Buffer", DOC(dai, Buffer));

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    // Call the rest of the type defines, then perform the actual bindings
    Callstack* callstack = (Callstack*) pCallstack;
    auto cb = callstack->top();
    callstack->pop();
    cb(m, pCallstack);
    // Actual bindings
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    // Metadata / raw
    rawBuffer
        .def(py::init<>())
        .def_property("data", [](py::object &obj){
            dai::RawBuffer &a = obj.cast<dai::RawBuffer&>();
            return py::array_t<uint8_t>(a.data.size(), a.data.data(), obj);
        }, [](py::object &obj, py::array_t<std::uint8_t, py::array::c_style> array){
            dai::RawBuffer &a = obj.cast<dai::RawBuffer&>();
            a.data = {array.data(), array.data() + array.size()};
        })
        ;

    // Message
    buffer
        .def(py::init<>(), DOC(dai, Buffer, Buffer))

        // obj is "Python" object, which we used then to bind the numpy arrays lifespan to
        .def("getData", [](py::object &obj){
            // creates numpy array (zero-copy) which holds correct information such as shape, ...
            dai::Buffer &a = obj.cast<dai::Buffer&>();
            return py::array_t<uint8_t>(a.getData().size(), a.getData().data(), obj);
        }, DOC(dai, Buffer, getData))
        .def("setData", py::overload_cast<const std::vector<std::uint8_t>&>(&Buffer::setData), DOC(dai, Buffer, setData))
        .def("setData", [](Buffer& buffer, py::array_t<std::uint8_t, py::array::c_style | py::array::forcecast> array){
            buffer.getData().clear();
            buffer.getData().insert(buffer.getData().begin(), array.data(), array.data() + array.nbytes());
        }, DOC(dai, Buffer, setData))
        ;


}
