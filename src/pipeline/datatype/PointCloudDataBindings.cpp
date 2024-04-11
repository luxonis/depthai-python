#include "DatatypeBindings.hpp"
#include "depthai-shared/datatype/RawPointCloudData.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/PointCloudData.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_pointclouddata(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawPointCloudData, RawBuffer, std::shared_ptr<RawPointCloudData>> rawPointCloudData(m, "RawPointCloudData", DOC(dai, RawPointCloudData));
    py::class_<PointCloudData, Buffer, std::shared_ptr<PointCloudData>> pointCloudData(m, "PointCloudData", DOC(dai, PointCloudData));

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
    rawPointCloudData
        .def(py::init<>())
        .def_readwrite("width", &RawPointCloudData::width)
        .def_readwrite("height", &RawPointCloudData::height)
        .def_readwrite("sparse", &RawPointCloudData::sparse)
        .def_readwrite("instanceNum", &RawPointCloudData::instanceNum)
        .def_readwrite("minx", &RawPointCloudData::minx)
        .def_readwrite("miny", &RawPointCloudData::miny)
        .def_readwrite("minz", &RawPointCloudData::minz)
        .def_readwrite("maxx", &RawPointCloudData::maxx)
        .def_readwrite("maxy", &RawPointCloudData::maxy)
        .def_readwrite("maxz", &RawPointCloudData::maxz)
        ;

    // Message
    pointCloudData
        .def(py::init<>())
        .def_property("points", [](PointCloudData& data) { return &data.getPoints(); }, [](PointCloudData& data, std::vector<Point3f> points) {data.getPoints() = points;})
        .def("getPoints", [](py::object &obj){
            // creates numpy array (zero-copy) which holds correct information such as shape, ...
            dai::PointCloudData& data = obj.cast<dai::PointCloudData&>();
            Point3f* points = (Point3f*)data.getData().data();
            unsigned long size = data.getData().size() / sizeof(Point3f);
            py::array_t<float> arr({size, 3UL});
            auto ra = arr.mutable_unchecked();
            for (int i = 0; i < size; i++) {
                ra(i, 0) = points[i].x;
                ra(i, 1) = points[i].y;
                ra(i, 2) = points[i].z;
            }
            return arr;
        })
        .def("getWidth", &PointCloudData::getWidth, DOC(dai, PointCloudData, getWidth))
        .def("getHeight", &PointCloudData::getHeight, DOC(dai, PointCloudData, getHeight))
        .def("isSparse", &PointCloudData::isSparse, DOC(dai, PointCloudData, isSparse))
        .def("getMinX", &PointCloudData::getMinX, DOC(dai, PointCloudData, getMinX))
        .def("getMinY", &PointCloudData::getMinY, DOC(dai, PointCloudData, getMinY))
        .def("getMinZ", &PointCloudData::getMinZ, DOC(dai, PointCloudData, getMinZ))
        .def("getMaxX", &PointCloudData::getMaxX, DOC(dai, PointCloudData, getMaxX))
        .def("getMaxY", &PointCloudData::getMaxY, DOC(dai, PointCloudData, getMaxY))
        .def("getMaxZ", &PointCloudData::getMaxZ, DOC(dai, PointCloudData, getMaxZ))
        .def("getInstanceNum", &PointCloudData::getInstanceNum, DOC(dai, PointCloudData, getInstanceNum))
        .def("getTimestamp", &PointCloudData::Buffer::getTimestamp, DOC(dai, Buffer, getTimestamp))
        .def("getTimestampDevice", &PointCloudData::Buffer::getTimestampDevice, DOC(dai, Buffer, getTimestampDevice))
        .def("getSequenceNum", &PointCloudData::Buffer::getSequenceNum, DOC(dai, Buffer, getSequenceNum))
        .def("setWidth", &PointCloudData::setWidth, DOC(dai, PointCloudData, setWidth))
        .def("setHeight", &PointCloudData::setHeight, DOC(dai, PointCloudData, setHeight))
        .def("setSize", static_cast<PointCloudData&(PointCloudData::*)(unsigned int, unsigned int)>(&PointCloudData::setSize), py::arg("width"), py::arg("height"), DOC(dai, PointCloudData, setSize))
        .def("setSize", static_cast<PointCloudData&(PointCloudData::*)(std::tuple<unsigned int, unsigned int>)>(&PointCloudData::setSize), py::arg("size"), DOC(dai, PointCloudData, setSize, 2))
        .def("setMinX", &PointCloudData::setMinX, DOC(dai, PointCloudData, setMinX))
        .def("setMinY", &PointCloudData::setMinY, DOC(dai, PointCloudData, setMinY))
        .def("setMinZ", &PointCloudData::setMinZ, DOC(dai, PointCloudData, setMinZ))
        .def("setMaxX", &PointCloudData::setMaxX, DOC(dai, PointCloudData, setMaxX))
        .def("setMaxY", &PointCloudData::setMaxY, DOC(dai, PointCloudData, setMaxY))
        .def("setMaxZ", &PointCloudData::setMaxZ, DOC(dai, PointCloudData, setMaxZ))
        .def("setInstanceNum", &PointCloudData::setInstanceNum, DOC(dai, PointCloudData, setInstanceNum))
        .def("setTimestamp", &PointCloudData::setTimestamp, DOC(dai, PointCloudData, setTimestamp))
        .def("setTimestampDevice", &PointCloudData::setTimestampDevice, DOC(dai, PointCloudData, setTimestampDevice))
        .def("setSequenceNum", &PointCloudData::setSequenceNum, DOC(dai, PointCloudData, setSequenceNum))
        ;

}
