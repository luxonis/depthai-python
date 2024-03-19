#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/ImageManip.hpp"

void bind_imagemanip(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    // TODO(themarpe) - properties
    auto imageManip = ADD_NODE(ImageManip);

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

    // ImageManip Node
    imageManip
        .def_readonly("inputConfig", &ImageManip::inputConfig, DOC(dai, node, ImageManip, inputConfig))
        .def_readonly("inputImage", &ImageManip::inputImage, DOC(dai, node, ImageManip, inputImage))
        .def_readonly("out", &ImageManip::out, DOC(dai, node, ImageManip, out))
        .def_readonly("initialConfig", &ImageManip::initialConfig, DOC(dai, node, ImageManip, initialConfig))
        // setters

        .def("setCropRect", [](ImageManip& im, float xmin, float ymin, float xmax, float ymax) {
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setCropRect() is deprecated, use initialConfig.setCropRect() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            im.setCropRect(xmin, ymin, xmax, ymax);
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("setCenterCrop", [](ImageManip& im, float ratio, float whRatio = 1.0f) {
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setCenterCrop() is deprecated, use initialConfig.setCenterCrop() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            im.setCenterCrop(ratio, whRatio);
            HEDLEY_DIAGNOSTIC_POP
        })

        .def("setResize", [](ImageManip& im, int w, int h) {
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setResize() is deprecated, use initialConfig.setResize() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            im.setResize(w, h);
            HEDLEY_DIAGNOSTIC_POP
        })

        .def("setResizeThumbnail", [](ImageManip& im, int w, int h, int bgRed = 0, int bgGreen = 0, int bgBlue = 0) {
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setResizeThumbnail() is deprecated, use initialConfig.setResizeThumbnail() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            im.setResizeThumbnail(w, h, bgRed, bgGreen, bgBlue);
            HEDLEY_DIAGNOSTIC_POP
        })

        .def("setFrameType", [](ImageManip& im, dai::ImgFrame::Type name) {
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setFrameType() is deprecated, use initialConfig.setFrameType() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            im.setFrameType(name);
            HEDLEY_DIAGNOSTIC_POP
        })

        .def("setHorizontalFlip", [](ImageManip& im, bool flip) {
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setHorizontalFlip() is deprecated, use initialConfig.setHorizontalFlip() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            im.setHorizontalFlip(flip);
            HEDLEY_DIAGNOSTIC_POP
        })

        .def("setKeepAspectRatio", &ImageManip::setKeepAspectRatio, DOC(dai, node, ImageManip, setKeepAspectRatio))

        .def("setWaitForConfigInput", [](ImageManip& obj, bool wait){
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "Use 'inputConfig.setWaitForMessage()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            obj.setWaitForConfigInput(wait);
            HEDLEY_DIAGNOSTIC_POP
        }, py::arg("wait"), DOC(dai, node, ImageManip, setWaitForConfigInput))

        .def("getWaitForConfigInput", [](ImageManip& obj){
            // Issue a deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "Use 'inputConfig.setWaitForMessage()' instead", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return obj.getWaitForConfigInput();
            HEDLEY_DIAGNOSTIC_POP
        }, DOC(dai, node, ImageManip, getWaitForConfigInput))

        .def("setNumFramesPool", &ImageManip::setNumFramesPool, DOC(dai, node, ImageManip, setNumFramesPool))
        .def("setMaxOutputFrameSize", &ImageManip::setMaxOutputFrameSize, DOC(dai, node, ImageManip, setMaxOutputFrameSize))

        .def("setWarpMesh", py::overload_cast<const std::vector<Point2f>&, int, int>(&ImageManip::setWarpMesh), DOC(dai, node, ImageManip, setWarpMesh))
        .def("setWarpMesh", py::overload_cast<const std::vector<std::pair<float,float>>&, int, int>(&ImageManip::setWarpMesh), DOC(dai, node, ImageManip, setWarpMesh))
        ;

}
