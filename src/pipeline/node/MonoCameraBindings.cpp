#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/MonoCamera.hpp"

void bind_monocamera(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    py::class_<MonoCameraProperties> monoCameraProperties(m, "MonoCameraProperties", DOC(dai, MonoCameraProperties));
    py::enum_<MonoCameraProperties::SensorResolution> monoCameraPropertiesSensorResolution(monoCameraProperties, "SensorResolution", DOC(dai, MonoCameraProperties, SensorResolution));
    auto monoCamera = ADD_NODE(MonoCamera);

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


    // Properties
    monoCameraPropertiesSensorResolution
        .value("THE_720_P", MonoCameraProperties::SensorResolution::THE_720_P)
        .value("THE_800_P", MonoCameraProperties::SensorResolution::THE_800_P)
        .value("THE_400_P", MonoCameraProperties::SensorResolution::THE_400_P)
        .value("THE_480_P", MonoCameraProperties::SensorResolution::THE_480_P)
        .value("THE_1200_P", MonoCameraProperties::SensorResolution::THE_1200_P)
        ;

    monoCameraProperties
        .def_readwrite("initialControl", &MonoCameraProperties::initialControl)
        .def_readwrite("boardSocket", &MonoCameraProperties::boardSocket)
        .def_readwrite("resolution", &MonoCameraProperties::resolution)
        .def_readwrite("fps", &MonoCameraProperties::fps)
        .def_readwrite("isp3aFps", &MonoCameraProperties::isp3aFps)
        .def_readwrite("numFramesPool", &MonoCameraProperties::numFramesPool)
        .def_readwrite("numFramesPoolRaw", &MonoCameraProperties::numFramesPoolRaw)
        .def_readwrite("eventFilter", &MonoCameraProperties::eventFilter)
        ;

    // Node
    monoCamera
        .def_readonly("inputControl", &MonoCamera::inputControl, DOC(dai, node, MonoCamera, inputControl))
        .def_readonly("out",  &MonoCamera::out, DOC(dai, node, MonoCamera, out))
        .def_readonly("raw",  &MonoCamera::raw, DOC(dai, node, MonoCamera, raw))
        .def_readonly("frameEvent",  &MonoCamera::frameEvent, DOC(dai, node, MonoCamera, frameEvent))
        .def_readonly("initialControl",  &MonoCamera::initialControl, DOC(dai, node, MonoCamera, initialControl))
        .def("setCamId", [](MonoCamera& c, int64_t id) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "setCamId() is deprecated, use setBoardSocket() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            c.setCamId(id);
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("getCamId", [](MonoCamera& c) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "getCamId() is deprecated, use getBoardSocket() instead.", 1);
            HEDLEY_DIAGNOSTIC_PUSH
            HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
            return c.getCamId();
            HEDLEY_DIAGNOSTIC_POP
        })
        .def("setBoardSocket", &MonoCamera::setBoardSocket, py::arg("boardSocket"), DOC(dai, node, MonoCamera, setBoardSocket))
        .def("getBoardSocket", &MonoCamera::getBoardSocket, DOC(dai, node, MonoCamera, getBoardSocket))
        .def("setImageOrientation", &MonoCamera::setImageOrientation, py::arg("imageOrientation"), DOC(dai, node, MonoCamera, setImageOrientation))
        .def("getImageOrientation", &MonoCamera::getImageOrientation, DOC(dai, node, MonoCamera, getImageOrientation))
        .def("setResolution", &MonoCamera::setResolution, py::arg("resolution"), DOC(dai, node, MonoCamera, setResolution))
        .def("getResolution", &MonoCamera::getResolution, DOC(dai, node, MonoCamera, getResolution))
        .def("setFrameEventFilter", &MonoCamera::setFrameEventFilter, py::arg("events"), DOC(dai, node, MonoCamera, setFrameEventFilter))
        .def("getFrameEventFilter", &MonoCamera::getFrameEventFilter, DOC(dai, node, MonoCamera, getFrameEventFilter))
        .def("setFps",        &MonoCamera::setFps, py::arg("fps"), DOC(dai, node, MonoCamera, setFps))
        .def("setIsp3aFps", [&monoCamera](MonoCamera &c, int isp3aFps) {
          PyErr_WarnEx(PyExc_DeprecationWarning,
                       "setIsp3aFps is unstable", 1);
          HEDLEY_DIAGNOSTIC_PUSH
          HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
          c.setIsp3aFps(isp3aFps);
          HEDLEY_DIAGNOSTIC_POP
        }, py::arg("isp3aFps"), DOC(dai, node, MonoCamera, setIsp3aFps))
        .def("getFps",        &MonoCamera::getFps, DOC(dai, node, MonoCamera, getFps))
        .def("getResolutionSize", &MonoCamera::getResolutionSize, DOC(dai, node, MonoCamera, getResolutionSize))
        .def("getResolutionWidth", &MonoCamera::getResolutionWidth, DOC(dai, node, MonoCamera, getResolutionWidth))
        .def("getResolutionHeight", &MonoCamera::getResolutionHeight, DOC(dai, node, MonoCamera, getResolutionHeight))
        .def("setNumFramesPool", &MonoCamera::setNumFramesPool, DOC(dai, node, MonoCamera, setNumFramesPool))
        .def("getNumFramesPool", &MonoCamera::getNumFramesPool, DOC(dai, node, MonoCamera, getNumFramesPool))
        .def("setRawNumFramesPool", &MonoCamera::setRawNumFramesPool, DOC(dai, node, MonoCamera, setRawNumFramesPool))
        .def("getRawNumFramesPool", &MonoCamera::getRawNumFramesPool, DOC(dai, node, MonoCamera, getRawNumFramesPool))
        .def("setCamera", &MonoCamera::setCamera, py::arg("name"), DOC(dai, node, MonoCamera, setCamera))
        .def("getCamera", &MonoCamera::getCamera, DOC(dai, node, MonoCamera, getCamera))

        .def("setRawOutputPacked", &MonoCamera::setRawOutputPacked, py::arg("packed"), DOC(dai, node, MonoCamera, setRawOutputPacked))
        ;
    // ALIAS
    daiNodeModule.attr("MonoCamera").attr("Properties") = monoCameraProperties;

}
