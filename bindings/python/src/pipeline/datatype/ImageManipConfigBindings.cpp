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

    // py::class_<RawImageManipConfig, RawBuffer, std::shared_ptr<RawImageManipConfig>> rawImageManipConfig(m, "RawImageManipConfig", DOC(dai, RawImageManipConfig));
    py::class_<ImageManipConfig, Buffer, std::shared_ptr<ImageManipConfig>> imageManipConfig(m, "ImageManipConfig", DOC(dai, ImageManipConfig));
    py::class_<ImageManipConfig::CropRect> imageManipConfigCropRect(imageManipConfig, "CropRect", DOC(dai, ImageManipConfig, CropRect));
    py::class_<ImageManipConfig::CropConfig> imageManipConfigCrop(imageManipConfig, "CropConfig", DOC(dai, ImageManipConfig, CropConfig));
    py::class_<ImageManipConfig::ResizeConfig>imageManipConfigResizeConfig(imageManipConfig, "ResizeConfig", DOC(dai, ImageManipConfig, ResizeConfig));
    py::class_<ImageManipConfig::FormatConfig> imageManipConfigFormatConfig(imageManipConfig, "FormatConfig", DOC(dai, ImageManipConfig, FormatConfig));

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
    imageManipConfig
        .def(py::init<>())
        .def_readwrite("enableFormat", &ImageManipConfig::enableFormat)
        .def_readwrite("enableResize", &ImageManipConfig::enableResize)
        .def_readwrite("enableCrop", &ImageManipConfig::enableCrop)
        .def_readwrite("cropConfig", &ImageManipConfig::cropConfig)
        .def_readwrite("resizeConfig", &ImageManipConfig::resizeConfig)
        .def_readwrite("formatConfig", &ImageManipConfig::formatConfig)
        .def_readwrite("interpolation", &ImageManipConfig::interpolation)
        ;

    imageManipConfigCropRect
        .def(py::init<>())
        .def_readwrite("xmin", &ImageManipConfig::CropRect::xmin)
        .def_readwrite("ymin", &ImageManipConfig::CropRect::ymin)
        .def_readwrite("xmax", &ImageManipConfig::CropRect::xmax)
        .def_readwrite("ymax", &ImageManipConfig::CropRect::ymax)
        ;

    imageManipConfigCrop
        .def(py::init<>())
        .def_readwrite("cropRect", &ImageManipConfig::CropConfig::cropRect)
        .def_readwrite("cropRotatedRect", &ImageManipConfig::CropConfig::cropRotatedRect)
        .def_readwrite("enableCenterCropRectangle", &ImageManipConfig::CropConfig::enableCenterCropRectangle)
        .def_readwrite("cropRatio", &ImageManipConfig::CropConfig::cropRatio)
        .def_readwrite("widthHeightAspectRatio", &ImageManipConfig::CropConfig::widthHeightAspectRatio)
        .def_readwrite("enableRotatedRect", &ImageManipConfig::CropConfig::enableRotatedRect)
        .def_readwrite("normalizedCoords", &ImageManipConfig::CropConfig::normalizedCoords)
        ;

    imageManipConfigResizeConfig
        .def(py::init<>())
        .def_readwrite("width", &ImageManipConfig::ResizeConfig::width)
        .def_readwrite("height", &ImageManipConfig::ResizeConfig::height)
        .def_readwrite("lockAspectRatioFill", &ImageManipConfig::ResizeConfig::lockAspectRatioFill)
        .def_readwrite("bgRed", &ImageManipConfig::ResizeConfig::bgRed)
        .def_readwrite("bgGreen", &ImageManipConfig::ResizeConfig::bgGreen)
        .def_readwrite("bgBlue", &ImageManipConfig::ResizeConfig::bgBlue)
        .def_readwrite("warpFourPoints", &ImageManipConfig::ResizeConfig::warpFourPoints)
        .def_readwrite("normalizedCoords", &ImageManipConfig::ResizeConfig::normalizedCoords)
        .def_readwrite("enableWarp4pt", &ImageManipConfig::ResizeConfig::enableWarp4pt)
        .def_readwrite("warpMatrix3x3", &ImageManipConfig::ResizeConfig::warpMatrix3x3)
        .def_readwrite("enableWarpMatrix", &ImageManipConfig::ResizeConfig::enableWarpMatrix)
        .def_readwrite("warpBorderReplicate", &ImageManipConfig::ResizeConfig::warpBorderReplicate)
        .def_readwrite("rotationAngleDeg", &ImageManipConfig::ResizeConfig::rotationAngleDeg)
        .def_readwrite("enableRotation", &ImageManipConfig::ResizeConfig::enableRotation)
        .def_readwrite("keepAspectRatio", &ImageManipConfig::ResizeConfig::keepAspectRatio)
        ;

    imageManipConfigFormatConfig
        .def(py::init<>())
        .def_readwrite("type", &ImageManipConfig::FormatConfig::type)
        .def_readwrite("flipHorizontal", &ImageManipConfig::FormatConfig::flipHorizontal)
        .def_readwrite("flipVertical", &ImageManipConfig::FormatConfig::flipVertical)
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
        // .def("set", &ImageManipConfig::set, py::arg("config"), DOC(dai, ImageManipConfig, set))
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
        // .def("get", &ImageManipConfig::get, DOC(dai, ImageManipConfig, get))
        .def("getInterpolation", &ImageManipConfig::getInterpolation, DOC(dai, ImageManipConfig, getInterpolation))
        ;


}
