#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/ImgFrame.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

void bind_imgframe(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawImgFrame, RawBuffer, std::shared_ptr<RawImgFrame>> rawImgFrame(m, "RawImgFrame", DOC(dai, RawImgFrame));
    py::enum_<RawImgFrame::Type> rawImgFrameType(rawImgFrame, "Type");
    py::class_<RawImgFrame::Specs> rawImgFrameSpecs(rawImgFrame, "Specs", DOC(dai, RawImgFrame, Specs));
    py::class_<ImgFrame, Buffer, std::shared_ptr<ImgFrame>> imgFrame(m, "ImgFrame", DOC(dai, ImgFrame));

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

    rawImgFrame
        .def(py::init<>())
        .def_readwrite("fb", &RawImgFrame::fb)
        .def_readwrite("category", &RawImgFrame::category)
        .def_readwrite("instanceNum", &RawImgFrame::instanceNum)
        .def_readwrite("sequenceNum", &RawImgFrame::sequenceNum)
        .def_property("ts",
            [](const RawImgFrame& o){
                double ts = o.ts.sec + o.ts.nsec / 1000000000.0;
                return ts;
            },
            [](RawImgFrame& o, double ts){
                o.ts.sec = ts;
                o.ts.nsec = (ts - o.ts.sec) * 1000000000.0;
            }
        )
        .def_property("tsDevice",
            [](const RawImgFrame& o){
                double ts = o.tsDevice.sec + o.tsDevice.nsec / 1000000000.0;
                return ts;
            },
            [](RawImgFrame& o, double ts){
                o.tsDevice.sec = ts;
                o.tsDevice.nsec = (ts - o.tsDevice.sec) * 1000000000.0;
            }
        )
        .def_readwrite("disp2DepthMultiplier", &RawImgFrame::disp2DepthMultiplier)
        ;


    rawImgFrameType
        .value("YUV422i", RawImgFrame::Type::YUV422i)
        .value("YUV444p", RawImgFrame::Type::YUV444p)
        .value("YUV420p", RawImgFrame::Type::YUV420p)
        .value("YUV422p", RawImgFrame::Type::YUV422p)
        .value("YUV400p", RawImgFrame::Type::YUV400p)
        .value("RGBA8888", RawImgFrame::Type::RGBA8888)
        .value("RGB161616", RawImgFrame::Type::RGB161616)
        .value("RGB888p", RawImgFrame::Type::RGB888p)
        .value("BGR888p", RawImgFrame::Type::BGR888p)
        .value("RGB888i", RawImgFrame::Type::RGB888i)
        .value("BGR888i", RawImgFrame::Type::BGR888i)
        .value("RGBF16F16F16p", RawImgFrame::Type::RGBF16F16F16p)
        .value("BGRF16F16F16p", RawImgFrame::Type::BGRF16F16F16p)
        .value("RGBF16F16F16i", RawImgFrame::Type::RGBF16F16F16i)
        .value("BGRF16F16F16i", RawImgFrame::Type::BGRF16F16F16i)
        .value("GRAY8", RawImgFrame::Type::GRAY8)
        .value("GRAYF16", RawImgFrame::Type::GRAYF16)
        .value("LUT2", RawImgFrame::Type::LUT2)
        .value("LUT4", RawImgFrame::Type::LUT4)
        .value("LUT16", RawImgFrame::Type::LUT16)
        .value("RAW16", RawImgFrame::Type::RAW16)
        .value("RAW14", RawImgFrame::Type::RAW14)
        .value("RAW12", RawImgFrame::Type::RAW12)
        .value("RAW10", RawImgFrame::Type::RAW10)
        .value("RAW8", RawImgFrame::Type::RAW8)
        .value("PACK10", RawImgFrame::Type::PACK10)
        .value("PACK12", RawImgFrame::Type::PACK12)
        .value("YUV444i", RawImgFrame::Type::YUV444i)
        .value("NV12", RawImgFrame::Type::NV12)
        .value("NV21", RawImgFrame::Type::NV21)
        .value("BITSTREAM", RawImgFrame::Type::BITSTREAM)
        .value("HDR", RawImgFrame::Type::HDR)
        .value("RAW32", RawImgFrame::Type::RAW32)
        .value("NONE", RawImgFrame::Type::NONE)
        ;

    rawImgFrameSpecs
        .def(py::init<>())
        .def_readwrite("type", &RawImgFrame::Specs::type)
        .def_readwrite("width", &RawImgFrame::Specs::width)
        .def_readwrite("height", &RawImgFrame::Specs::height)
        .def_readwrite("stride", &RawImgFrame::Specs::stride)
        .def_readwrite("bytesPP", &RawImgFrame::Specs::bytesPP)
        .def_readwrite("p1Offset", &RawImgFrame::Specs::p1Offset)
        .def_readwrite("p2Offset", &RawImgFrame::Specs::p2Offset)
        .def_readwrite("p3Offset", &RawImgFrame::Specs::p3Offset)
        ;

    // TODO add RawImgFrame::CameraSettings

    // Message
        imgFrame
        .def(py::init<>())
        .def(py::init<size_t>())
        // getters
        .def("getTimestamp", py::overload_cast<>(&ImgFrame::Buffer::getTimestamp, py::const_), DOC(dai, Buffer, getTimestamp))
        .def("getTimestampDevice", py::overload_cast<>(&ImgFrame::Buffer::getTimestampDevice, py::const_), DOC(dai, Buffer, getTimestampDevice))
        .def("getTimestamp", py::overload_cast<CameraExposureOffset>(&ImgFrame::getTimestamp, py::const_), py::arg("offset"), DOC(dai, ImgFrame, getTimestamp))
        .def("getTimestampDevice", py::overload_cast<CameraExposureOffset>(&ImgFrame::getTimestampDevice, py::const_), py::arg("offset"), DOC(dai, ImgFrame, getTimestampDevice))
        .def("getSequenceNum", &ImgFrame::Buffer::getSequenceNum, DOC(dai, Buffer, getSequenceNum))
        .def("getInstanceNum", &ImgFrame::getInstanceNum, DOC(dai, ImgFrame, getInstanceNum))
        .def("getCategory", &ImgFrame::getCategory, DOC(dai, ImgFrame, getCategory))
        .def("getWidth", &ImgFrame::getWidth, DOC(dai, ImgFrame, getWidth))
        .def("getStride", &ImgFrame::getStride, DOC(dai, ImgFrame, getStride))
        .def("getHeight", &ImgFrame::getHeight, DOC(dai, ImgFrame, getHeight))
        .def("getPlaneStride", &ImgFrame::getPlaneStride, DOC(dai, ImgFrame, getPlaneStride))
        .def("getPlaneHeight", &ImgFrame::getPlaneHeight, DOC(dai, ImgFrame, getPlaneHeight))
        .def("getType", &ImgFrame::getType, DOC(dai, ImgFrame, getType))
        .def("getBytesPerPixel", &ImgFrame::getBytesPerPixel, DOC(dai, ImgFrame, getBytesPerPixel))
        .def("getExposureTime", &ImgFrame::getExposureTime, DOC(dai, ImgFrame, getExposureTime))
        .def("getSensitivity", &ImgFrame::getSensitivity, DOC(dai, ImgFrame, getSensitivity))
        .def("getColorTemperature", &ImgFrame::getColorTemperature, DOC(dai, ImgFrame, getColorTemperature))
        .def("getLensPosition", &ImgFrame::getLensPosition, DOC(dai, ImgFrame, getLensPosition))
        .def("get", &ImgFrame::get, DOC(dai, ImgFrame, get))
        .def("getSensorTemperature", &ImgFrame::getSensorTemperature, DOC(dai, ImgFrame, getSensorTemperature))
        .def("getAuxTemperature", &ImgFrame::getAuxTemperature, DOC(dai, ImgFrame, getAuxTemperature))

        // OpenCV Support section
        .def("setFrame", [](dai::ImgFrame& frm, py::array arr){
             // Try importing 'numpy' module
            py::module numpy;
            try {
                numpy = py::module::import("numpy");
            } catch (const py::error_already_set& err){
                throw std::runtime_error("Function 'setFrame' requires 'numpy' module");
            }

            py::array contiguous = numpy.attr("ascontiguousarray")(arr);
            frm.setData({(uint8_t*) contiguous.data(), (uint8_t*) contiguous.data() + contiguous.nbytes()});
            // frm.getData().resize(contiguous.nbytes());
            // memcpy(frm.getData().data(), contiguous.data(), contiguous.nbytes());

        }, py::arg("array"), "Copies array bytes to ImgFrame buffer")
        .def("getFrame", [](py::object &obj, bool copy){

            // Try importing 'numpy' module
            py::module numpy;
            try {
                numpy = py::module::import("numpy");
            } catch (const py::error_already_set& err){
                throw std::runtime_error("Function 'getFrame' requires 'numpy' module");
            }

            // obj is "Python" object, which we used then to bind the numpy view lifespan to
            // creates numpy array (zero-copy) which holds correct information such as shape, ...
            auto& img = obj.cast<dai::ImgFrame&>();

            // shape
            bool valid = img.getWidth() > 0 && img.getHeight() > 0;
            std::vector<std::size_t> shape = {img.getData().size()};
            std::vector<std::size_t> strides = {};
            py::dtype dtype = py::dtype::of<uint8_t>();

            switch(img.getType()){

                case ImgFrame::Type::RGB888i :
                case ImgFrame::Type::BGR888i :
                    // HWC
                    shape = {img.getHeight(), img.getWidth(), 3};
                    dtype = py::dtype::of<uint8_t>();
                break;

                case ImgFrame::Type::RGB888p :
                case ImgFrame::Type::BGR888p :
                    // CHW
                    shape = {3, img.getHeight(), img.getWidth()};
                    dtype = py::dtype::of<uint8_t>();
                break;

                case ImgFrame::Type::YUV420p:
                case ImgFrame::Type::NV12:
                case ImgFrame::Type::NV21:
                    // Height 1.5x actual size
                    shape = {img.getPlaneHeight() * 3 / 2, img.getWidth()};
                    strides = {img.getStride(), 1};
                    dtype = py::dtype::of<uint8_t>();
                break;

                case ImgFrame::Type::RAW8:
                case ImgFrame::Type::GRAY8:
                    shape = {img.getHeight(), img.getWidth()};
                    dtype = py::dtype::of<uint8_t>();
                break;

                case ImgFrame::Type::GRAYF16:
                    shape = {img.getHeight(), img.getWidth()};
                    dtype = py::dtype("half");
                break;

                case ImgFrame::Type::RAW16:
                case ImgFrame::Type::RAW14:
                case ImgFrame::Type::RAW12:
                case ImgFrame::Type::RAW10:
                    shape = {img.getHeight(), img.getWidth()};
                    dtype = py::dtype::of<uint16_t>();
                break;

                case ImgFrame::Type::RGBF16F16F16i:
                case ImgFrame::Type::BGRF16F16F16i:
                    shape = {img.getHeight(), img.getWidth(), 3};
                    dtype = py::dtype("half");
                break;

                case ImgFrame::Type::RGBF16F16F16p:
                case ImgFrame::Type::BGRF16F16F16p:
                    shape = {3, img.getHeight(), img.getWidth()};
                    dtype = py::dtype("half");
                break;

                case ImgFrame::Type::BITSTREAM :
                default:
                    shape = {img.getData().size()};
                    dtype = py::dtype::of<uint8_t>();
                    break;
            }

            // Check if enough data
            long actualSize = img.getData().size();
            long requiredSize = dtype.itemsize();
            for(const auto& dim : shape) requiredSize *= dim;
            if(actualSize < requiredSize){
                throw std::runtime_error("ImgFrame doesn't have enough data to encode specified frame, required " + std::to_string(requiredSize)
                        + ", actual " + std::to_string(actualSize) + ". Maybe metadataOnly transfer was made?");
            } else if(actualSize > requiredSize) {
                // FIXME check build on Windows
                // logger::warn("ImgFrame has excess data: actual {}, expected {}", actualSize, requiredSize);
            }
            if(img.getWidth() <= 0 || img.getHeight() <= 0){
                throw std::runtime_error("ImgFrame size invalid (width: " + std::to_string(img.getWidth()) + ", height: " + std::to_string(img.getHeight()) + ")");
            }

            if(copy){
                py::array a(dtype, shape);
                std::memcpy(a.mutable_data(), img.getData().data(), std::min( (long) (img.getData().size()), (long) (a.nbytes())));
                // TODO handle strides
                return a;
            } else {
                return py::array(dtype, shape, strides, img.getData().data(), obj);
            }

        }, py::arg("copy") = false, "Returns numpy array with shape as specified by width, height and type")

        .def("getCvFrame", [](py::object &obj){
            using namespace pybind11::literals;

            // Try importing 'cv2' module
            py::module cv2;
            py::module numpy;
            try {
                cv2 = py::module::import("cv2");
                numpy = py::module::import("numpy");
            } catch (const py::error_already_set& err){
                throw std::runtime_error("Function 'getCvFrame' requires 'cv2' module (opencv-python package)");
            }

            // ImgFrame
            auto& img = obj.cast<dai::ImgFrame&>();

            // Get numpy frame (python object) by calling getFrame
            auto frame = obj.attr("getFrame")();

            // Convert numpy array to bgr frame using cv2 module
            switch(img.getType()) {

                case ImgFrame::Type::BGR888p:
                    return numpy.attr("ascontiguousarray")(frame.attr("transpose")(1, 2, 0));
                    break;

                case ImgFrame::Type::BGR888i:
                    return frame.attr("copy")();
                    break;

                case ImgFrame::Type::RGB888p:
                    // Transpose to RGB888i then convert to BGR
                    return cv2.attr("cvtColor")(frame.attr("transpose")(1, 2, 0), cv2.attr("COLOR_RGB2BGR"));
                    break;

                case ImgFrame::Type::RGB888i:
                    return cv2.attr("cvtColor")(frame, cv2.attr("COLOR_RGB2BGR"));
                    break;

                case ImgFrame::Type::YUV420p:
                    return cv2.attr("cvtColor")(frame, cv2.attr("COLOR_YUV2BGR_IYUV"));
                    break;

                case ImgFrame::Type::NV12:
                case ImgFrame::Type::NV21: {
                    auto code = (img.getType() == ImgFrame::Type::NV12) ? cv2.attr("COLOR_YUV2BGR_NV12") : cv2.attr("COLOR_YUV2BGR_NV21");
                    if(img.getPlaneHeight() <= img.getHeight()) {
                        return cv2.attr("cvtColor")(frame, code);
                    } else {
                        py::dtype dtype = py::dtype::of<uint8_t>();
                        std::vector<std::size_t> shapeY = {img.getHeight(), img.getWidth()};
                        std::vector<std::size_t> shapeUV = {img.getHeight() / 2, img.getWidth() / 2};
                        std::vector<std::size_t> strides = {img.getStride(), 1};
                        auto frameY = py::array(dtype, shapeY, strides, img.getData().data(), obj);
                        auto frameUV = py::array(dtype, shapeUV, strides, img.getData().data() + img.getPlaneStride(), obj);
                        return cv2.attr("cvtColorTwoPlane")(frameY, frameUV, code);
                    }
                } break;

                case ImgFrame::Type::RAW8:
                case ImgFrame::Type::RAW16:
                case ImgFrame::Type::RAW14:
                case ImgFrame::Type::RAW12:
                case ImgFrame::Type::RAW10:
                case ImgFrame::Type::GRAY8:
                case ImgFrame::Type::GRAYF16:
                default:
                    return frame.attr("copy")();
                    break;
            }

            // Default case
            return frame.attr("copy")();

        }, "Returns BGR or grayscale frame compatible with use in other opencv functions")

        // setters
        .def("setTimestamp", &ImgFrame::setTimestamp, py::arg("timestamp"), DOC(dai, ImgFrame, setTimestamp))
        .def("setTimestampDevice", &ImgFrame::setTimestampDevice, DOC(dai, ImgFrame, setTimestampDevice))
        .def("setInstanceNum", &ImgFrame::setInstanceNum, py::arg("instance"), DOC(dai, ImgFrame, setInstanceNum))
        .def("setCategory", &ImgFrame::setCategory, py::arg("category"), DOC(dai, ImgFrame, setCategory))
        .def("setSequenceNum", &ImgFrame::setSequenceNum, py::arg("seq"), DOC(dai, ImgFrame, setSequenceNum))
        .def("setWidth", &ImgFrame::setWidth, py::arg("width"), DOC(dai, ImgFrame, setWidth))
        .def("setHeight", &ImgFrame::setHeight, py::arg("height"), DOC(dai, ImgFrame, setHeight))
        .def("setSize", static_cast<ImgFrame&(ImgFrame::*)(unsigned int, unsigned int)>(&ImgFrame::setSize), py::arg("width"), py::arg("height"), DOC(dai, ImgFrame, setSize))
        .def("setSize", static_cast<ImgFrame&(ImgFrame::*)(std::tuple<unsigned int, unsigned int>)>(&ImgFrame::setSize), py::arg("sizer"), DOC(dai, ImgFrame, setSize, 2))
        .def("setType", &ImgFrame::setType, py::arg("type"), DOC(dai, ImgFrame, setType))
        .def("set", &ImgFrame::set, py::arg("type"), DOC(dai, ImgFrame, set))
        ;
    // add aliases dai.ImgFrame.Type and dai.ImgFrame.Specs
    m.attr("ImgFrame").attr("Type") = m.attr("RawImgFrame").attr("Type");
    m.attr("ImgFrame").attr("Specs") = m.attr("RawImgFrame").attr("Specs");

}
