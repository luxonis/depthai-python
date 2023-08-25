#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/EncodedFrame.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

void bind_encodedframe(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawEncodedFrame, RawBuffer, std::shared_ptr<RawEncodedFrame>> rawEncodedFrame(m, "RawEncodedFrame", DOC(dai, RawEncodedFrame));
    py::enum_<RawEncodedFrame::Profile> rawEncodedFrameProfile(rawEncodedFrame, "Profile");
    py::enum_<RawEncodedFrame::FrameType> rawEncodedFrameType(rawEncodedFrame, "FrameType", DOC(dai, RawEncodedFrame, FrameType));
    py::class_<EncodedFrame, Buffer, std::shared_ptr<EncodedFrame>> encodedFrame(m, "EncodedFrame", DOC(dai, EncodedFrame));

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

    rawEncodedFrame
        .def(py::init<>())
        .def_readwrite("height", &RawEncodedFrame::height)
        .def_readwrite("width", &RawEncodedFrame::width)
        .def_readwrite("quality", &RawEncodedFrame::quality)
        .def_readwrite("profile", &RawEncodedFrame::profile)
        .def_readwrite("lossless", &RawEncodedFrame::lossless)
        .def_readwrite("type", &RawEncodedFrame::type)
        .def_readwrite("sequenceNum", &RawEncodedFrame::sequenceNum)
        .def_property("ts",
            [](const RawEncodedFrame& o){
                double ts = o.ts.sec + o.ts.nsec / 1000000000.0;
                return ts;
            },
            [](RawEncodedFrame& o, double ts){
                o.ts.sec = ts;
                o.ts.nsec = (ts - o.ts.sec) * 1000000000.0;
            }
        )
        .def_property("tsDevice",
            [](const RawEncodedFrame& o){
                double ts = o.tsDevice.sec + o.tsDevice.nsec / 1000000000.0;
                return ts;
            },
            [](RawEncodedFrame& o, double ts){
                o.tsDevice.sec = ts;
                o.tsDevice.nsec = (ts - o.tsDevice.sec) * 1000000000.0;
            }
        )
        ;


    rawEncodedFrameProfile
        .value("JPEG", EncodedFrame::Profile::JPEG)
        .value("AVC", EncodedFrame::Profile::AVC)
        .value("HEVC", EncodedFrame::Profile::HEVC)
        ;

    rawEncodedFrameType
        .value("I", EncodedFrame::FrameType::I)
        .value("P", EncodedFrame::FrameType::P)
        .value("B", EncodedFrame::FrameType::B)
        ;

    // Message
    encodedFrame
        .def(py::init<>())
        // getters
        .def("getTimestamp", py::overload_cast<>(&EncodedFrame::getTimestamp, py::const_), DOC(dai, EncodedFrame, getTimestamp))
        .def("getTimestampDevice", py::overload_cast<>(&EncodedFrame::getTimestampDevice, py::const_), DOC(dai, EncodedFrame, getTimestampDevice))
        .def("getSequenceNum", &EncodedFrame::getSequenceNum, DOC(dai, EncodedFrame, getSequenceNum))
        .def("getWidth", &EncodedFrame::getWidth, DOC(dai, EncodedFrame, getWidth))
        .def("getHeight", &EncodedFrame::getHeight, DOC(dai, EncodedFrame, getHeight))
        .def("getQuality", &EncodedFrame::getQuality, DOC(dai, EncodedFrame, getQuality))
        .def("getFrameType", &EncodedFrame::getFrameType, DOC(dai, EncodedFrame, getFrameType))
        .def("getLossless", &EncodedFrame::getLossless, DOC(dai, EncodedFrame, getLossless))
        .def("getProfile", &EncodedFrame::getProfile, DOC(dai, EncodedFrame, getProfile))

        // setters
        .def("setTimestamp", &EncodedFrame::setTimestamp, DOC(dai, EncodedFrame, setTimestamp))
        .def("setTimestampDevice", &EncodedFrame::setTimestampDevice, DOC(dai, EncodedFrame, setTimestampDevice))
        .def("setSequenceNum", &EncodedFrame::setSequenceNum, DOC(dai, EncodedFrame, getSequenceNum))
        .def("setWidth", &EncodedFrame::setWidth, DOC(dai, EncodedFrame, getWidth))
        .def("setHeight", &EncodedFrame::setHeight, DOC(dai, EncodedFrame, getHeight))
        .def("setSize", static_cast<EncodedFrame&(EncodedFrame::*)(unsigned int, unsigned int)>(&EncodedFrame::setSize), py::arg("width"), py::arg("height"), DOC(dai, EncodedFrame, setSize))
        .def("setSize", static_cast<EncodedFrame&(EncodedFrame::*)(std::tuple<unsigned int, unsigned int>)>(&EncodedFrame::setSize), py::arg("size"), DOC(dai, EncodedFrame, setSize, 2))
        .def("setQuality", &EncodedFrame::setQuality, DOC(dai, EncodedFrame, getQuality))
        .def("setFrameType", &EncodedFrame::setFrameType, DOC(dai, EncodedFrame, getFrameType))
        .def("setLossless", &EncodedFrame::setLossless, DOC(dai, EncodedFrame, getLossless))
        .def("setProfile", &EncodedFrame::setProfile, DOC(dai, EncodedFrame, getProfile))
        ;
    // add aliases dai.ImgFrame.Type and dai.ImgFrame.Specs
    m.attr("EncodedFrame").attr("FrameType") = m.attr("RawEncodedFrame").attr("FrameType");
    m.attr("EncodedFrame").attr("Profile") = m.attr("RawEncodedFrame").attr("Profile");
}
