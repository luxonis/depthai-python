#include "DatatypeBindings.hpp"
#include "pipeline/CommonBindings.hpp"
#include <unordered_map>
#include <memory>

// depthai
#include "depthai/pipeline/datatype/ImageManipConfig.hpp"

//pybind
#include <pybind11/chrono.h>
#include <pybind11/numpy.h>

// #include "spdlog/spdlog.h"

void bind_imagemanipconfig(pybind11::module& m, void* pCallstack){

    using namespace dai;

    py::class_<RawImageManipConfig, RawBuffer, std::shared_ptr<RawImageManipConfig>> rawImageManipConfig(m, "RawImageManipConfig", DOC(dai, RawImageManipConfig));
    py::class_<RawImageManipConfig::CropRect> rawImageManipConfigCropRect(rawImageManipConfig, "CropRect", DOC(dai, RawImageManipConfig, CropRect));
    py::class_<RawImageManipConfig::CropConfig> rawImageManipCropConfig(rawImageManipConfig, "CropConfig", DOC(dai, RawImageManipConfig, CropConfig));
    py::class_<RawImageManipConfig::ResizeConfig>rawImageManipConfigResizeConfig(rawImageManipConfig, "ResizeConfig", DOC(dai, RawImageManipConfig, ResizeConfig));
    py::class_<RawImageManipConfig::FormatConfig> rawImageManipConfigFormatConfig(rawImageManipConfig, "FormatConfig", DOC(dai, RawImageManipConfig, FormatConfig));
    py::class_<ImageManipConfig, Buffer, std::shared_ptr<ImageManipConfig>> imageManipConfig(m, "ImageManipConfig", DOC(dai, ImageManipConfig));

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
    rawImageManipConfig
        .def(py::init<>())
        .def_readwrite("enableFormat", &RawImageManipConfig::enableFormat)
        .def_readwrite("enableResize", &RawImageManipConfig::enableResize)
        .def_readwrite("enableCrop", &RawImageManipConfig::enableCrop)
        .def_readwrite("cropConfig", &RawImageManipConfig::cropConfig)
        .def_readwrite("resizeConfig", &RawImageManipConfig::resizeConfig)
        .def_readwrite("formatConfig", &RawImageManipConfig::formatConfig)
        .def_readwrite("interpolation", &RawImageManipConfig::interpolation)
        ;

    rawImageManipConfigCropRect
        .def(py::init<>())
        .def_readwrite("xmin", &RawImageManipConfig::CropRect::xmin)
        .def_readwrite("ymin", &RawImageManipConfig::CropRect::ymin)
        .def_readwrite("xmax", &RawImageManipConfig::CropRect::xmax)
        .def_readwrite("ymax", &RawImageManipConfig::CropRect::ymax)
        ;

    rawImageManipCropConfig
        .def(py::init<>())
        .def_readwrite("cropRect", &RawImageManipConfig::CropConfig::cropRect)
        .def_readwrite("cropRotatedRect", &RawImageManipConfig::CropConfig::cropRotatedRect)
        .def_readwrite("enableCenterCropRectangle", &RawImageManipConfig::CropConfig::enableCenterCropRectangle)
        .def_readwrite("cropRatio", &RawImageManipConfig::CropConfig::cropRatio)
        .def_readwrite("widthHeightAspectRatio", &RawImageManipConfig::CropConfig::widthHeightAspectRatio)
        .def_readwrite("enableRotatedRect", &RawImageManipConfig::CropConfig::enableRotatedRect)
        .def_readwrite("normalizedCoords", &RawImageManipConfig::CropConfig::normalizedCoords)
        ;

    rawImageManipConfigResizeConfig
        .def(py::init<>())
        .def_readwrite("width", &RawImageManipConfig::ResizeConfig::width)
        .def_readwrite("height", &RawImageManipConfig::ResizeConfig::height)
        .def_readwrite("lockAspectRatioFill", &RawImageManipConfig::ResizeConfig::lockAspectRatioFill)
        .def_readwrite("bgRed", &RawImageManipConfig::ResizeConfig::bgRed)
        .def_readwrite("bgGreen", &RawImageManipConfig::ResizeConfig::bgGreen)
        .def_readwrite("bgBlue", &RawImageManipConfig::ResizeConfig::bgBlue)
        .def_readwrite("warpFourPoints", &RawImageManipConfig::ResizeConfig::warpFourPoints)
        .def_readwrite("normalizedCoords", &RawImageManipConfig::ResizeConfig::normalizedCoords)
        .def_readwrite("enableWarp4pt", &RawImageManipConfig::ResizeConfig::enableWarp4pt)
        .def_readwrite("warpMatrix3x3", &RawImageManipConfig::ResizeConfig::warpMatrix3x3)
        .def_readwrite("enableWarpMatrix", &RawImageManipConfig::ResizeConfig::enableWarpMatrix)
        .def_readwrite("warpBorderReplicate", &RawImageManipConfig::ResizeConfig::warpBorderReplicate)
        .def_readwrite("rotationAngleDeg", &RawImageManipConfig::ResizeConfig::rotationAngleDeg)
        .def_readwrite("enableRotation", &RawImageManipConfig::ResizeConfig::enableRotation)
        .def_readwrite("keepAspectRatio", &RawImageManipConfig::ResizeConfig::keepAspectRatio)
        ;

    rawImageManipConfigFormatConfig
        .def(py::init<>())
        .def_readwrite("type", &RawImageManipConfig::FormatConfig::type)
        .def_readwrite("flipHorizontal", &RawImageManipConfig::FormatConfig::flipHorizontal)
        .def_readwrite("flipVertical", &RawImageManipConfig::FormatConfig::flipVertical)
        ;

    // Message

    imageManipConfig
        .def(py::init<>())
        // setters
        .def("setCropRect", static_cast<ImageManipConfig&(ImageManipConfig::*)(float, float, float, float)>(&ImageManipConfig::setCropRect), py::arg("xmin"), py::arg("ymin"), py::arg("xmax"), py::arg("ymax"), DOC(dai, ImageManipConfig, setCropRect))
        .def("setCropRect", static_cast<ImageManipConfig&(ImageManipConfig::*)(std::tuple<float, float, float, float>)>(&ImageManipConfig::setCropRect), py::arg("coordinates"), DOC(dai, ImageManipConfig, setCropRect, 2))
        .def("setCropRotatedRect", &ImageManipConfig::setCropRotatedRect, py::arg("rr"), py::arg("normalizedCoords") = true, DOC(dai, ImageManipConfig, setCropRotatedRect))
        .def("setCenterCrop", &ImageManipConfig::setCenterCrop, py::arg("ratio"), py::arg("whRatio")=1.0f, DOC(dai, ImageManipConfig, setCenterCrop))
        .def("setWarpTransformFourPoints", &ImageManipConfig::setWarpTransformFourPoints, py::arg("pt"), py::arg("normalizedCoords"), DOC(dai, ImageManipConfig, setWarpTransformFourPoints))
        .def("setWarpTransformMatrix3x3", &ImageManipConfig::setWarpTransformMatrix3x3, py::arg("mat"), DOC(dai, ImageManipConfig, setWarpTransformMatrix3x3))
        .def("setWarpBorderReplicatePixels", &ImageManipConfig::setWarpBorderReplicatePixels, DOC(dai, ImageManipConfig, setWarpBorderReplicatePixels))
        .def("setWarpBorderFillColor", &ImageManipConfig::setWarpBorderFillColor, py::arg("red"), py::arg("green"), py::arg("blue"), DOC(dai, ImageManipConfig, setWarpBorderFillColor))
        .def("setRotationDegrees", &ImageManipConfig::setRotationDegrees, py::arg("deg"), DOC(dai, ImageManipConfig, setRotationDegrees))
        .def("setRotationRadians", &ImageManipConfig::setRotationRadians, py::arg("rad"), DOC(dai, ImageManipConfig, setRotationRadians))
        .def("setResize", static_cast<ImageManipConfig&(ImageManipConfig::*)(int, int)>(&ImageManipConfig::setResize), py::arg("w"), py::arg("h"), DOC(dai, ImageManipConfig, setResize))
        .def("setResize", static_cast<ImageManipConfig&(ImageManipConfig::*)(std::tuple<int, int>)>(&ImageManipConfig::setResize), py::arg("size"), DOC(dai, ImageManipConfig, setResize, 2))
        .def("setResizeThumbnail", static_cast<ImageManipConfig&(ImageManipConfig::*)(int, int, int, int, int)>(&ImageManipConfig::setResizeThumbnail), py::arg("w"), py::arg("h"), py::arg("bgRed")=0, py::arg("bgGreen")=0, py::arg("bgBlue")=0, DOC(dai, ImageManipConfig, setResizeThumbnail))
        .def("setResizeThumbnail", static_cast<ImageManipConfig&(ImageManipConfig::*)(std::tuple<int, int>, int, int, int)>(&ImageManipConfig::setResizeThumbnail), py::arg("size"), py::arg("bgRed")=0, py::arg("bgGreen")=0, py::arg("bgBlue")=0, DOC(dai, ImageManipConfig, setResizeThumbnail, 2))
        .def("setFrameType", &ImageManipConfig::setFrameType, py::arg("type"), DOC(dai, ImageManipConfig, setFrameType))
        .def("setColormap", static_cast<ImageManipConfig&(ImageManipConfig::*)(Colormap, int, int)>(&ImageManipConfig::setColormap), py::arg("colormap"), py::arg("min"), py::arg("max"), DOC(dai, ImageManipConfig, setColormap))
        .def("setColormap", static_cast<ImageManipConfig&(ImageManipConfig::*)(Colormap, int)>(&ImageManipConfig::setColormap), py::arg("colormap"), py::arg("max") = 255, DOC(dai, ImageManipConfig, setColormap))
        .def("setColormap", static_cast<ImageManipConfig&(ImageManipConfig::*)(Colormap, float)>(&ImageManipConfig::setColormap), py::arg("colormap"), py::arg("max") = 255.0f, DOC(dai, ImageManipConfig, setColormap))
        .def("setHorizontalFlip", &ImageManipConfig::setHorizontalFlip, py::arg("flip"), DOC(dai, ImageManipConfig, setHorizontalFlip))
        .def("setVerticalFlip", &ImageManipConfig::setVerticalFlip, py::arg("flip"), DOC(dai, ImageManipConfig, setVerticalFlip))
        .def("setReusePreviousImage", &ImageManipConfig::setReusePreviousImage, py::arg("reuse"), DOC(dai, ImageManipConfig, setReusePreviousImage))
        .def("setSkipCurrentImage", &ImageManipConfig::setSkipCurrentImage, py::arg("skip"), DOC(dai, ImageManipConfig, setSkipCurrentImage))
        .def("setKeepAspectRatio", &ImageManipConfig::setKeepAspectRatio, py::arg("keep"), DOC(dai, ImageManipConfig, setKeepAspectRatio))
        .def("set", &ImageManipConfig::set, py::arg("config"), DOC(dai, ImageManipConfig, set))
        .def("setInterpolation", &ImageManipConfig::setInterpolation, py::arg("interpolation"), DOC(dai, ImageManipConfig, setInterpolation))

        // getters
        .def("getCropXMin", &ImageManipConfig::getCropXMin, DOC(dai, ImageManipConfig, getCropXMin))
        .def("getCropYMin", &ImageManipConfig::getCropYMin, DOC(dai, ImageManipConfig, getCropYMin))
        .def("getCropXMax", &ImageManipConfig::getCropXMax, DOC(dai, ImageManipConfig, getCropXMax))
        .def("getCropYMax", &ImageManipConfig::getCropYMax, DOC(dai, ImageManipConfig, getCropYMax))
        .def("getResizeWidth", &ImageManipConfig::getResizeWidth, DOC(dai, ImageManipConfig, getResizeWidth))
        .def("getResizeHeight", &ImageManipConfig::getResizeHeight, DOC(dai, ImageManipConfig, getResizeHeight))
        .def("getCropConfig", &ImageManipConfig::getCropConfig, DOC(dai, ImageManipConfig, getCropConfig))
        .def("getResizeConfig", &ImageManipConfig::getResizeConfig, DOC(dai, ImageManipConfig, getResizeConfig))
        .def("getFormatConfig", &ImageManipConfig::getFormatConfig, DOC(dai, ImageManipConfig, getFormatConfig))
        .def("isResizeThumbnail", &ImageManipConfig::isResizeThumbnail, DOC(dai, ImageManipConfig, isResizeThumbnail))
        .def("getColormap", &ImageManipConfig::getColormap, DOC(dai, ImageManipConfig, getColormap))
        .def("get", &ImageManipConfig::get, DOC(dai, ImageManipConfig, get))
        .def("getInterpolation", &ImageManipConfig::getInterpolation, DOC(dai, ImageManipConfig, getInterpolation))
        ;


}
