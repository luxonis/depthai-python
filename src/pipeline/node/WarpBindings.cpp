#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/Warp.hpp"

void bind_warp(pybind11::module& m, void* pCallstack){

    using namespace dai;
    using namespace dai::node;

    // Node and Properties declare upfront
    // TODO(themarpe) - properties
    py::class_<Warp::Properties> warpProperties(m, "WarpProperties", DOC(dai, WarpProperties));
    auto warp = ADD_NODE(Warp);

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

    // properties

    // ImageManip Node
    warp
        // .def_readonly("inputConfig", &Warp::inputConfig, DOC(dai, node, Warp, inputConfig))
        .def_readonly("inputImage", &Warp::inputImage, DOC(dai, node, Warp, inputImage))
        .def_readonly("out", &Warp::out, DOC(dai, node, Warp, out))
        // .def_readonly("initialConfig", &Warp::initialConfig, DOC(dai, node, Warp, initialConfig))
        // setters

        .def("setOutputSize",  py::overload_cast<int, int>(&Warp::setOutputSize), DOC(dai, node, Warp, setOutputSize))
        .def("setOutputSize",  py::overload_cast<std::tuple<int, int>>(&Warp::setOutputSize), DOC(dai, node, Warp, setOutputSize, 2))
        // .def("setOutputWidth", &Warp::setOutputWidth, DOC(dai, node, Warp, setOutputWidth))
        // .def("setOutputHeight", &Warp::setOutputHeight, DOC(dai, node, Warp, setOutputHeight))

        .def("setNumFramesPool", &Warp::setNumFramesPool, DOC(dai, node, Warp, setNumFramesPool))
        .def("setMaxOutputFrameSize", &Warp::setMaxOutputFrameSize, DOC(dai, node, Warp, setMaxOutputFrameSize))

        .def("setWarpMesh", py::overload_cast<const std::vector<Point2f>&, int, int>(&Warp::setWarpMesh), DOC(dai, node, Warp, setWarpMesh))
        .def("setWarpMesh", py::overload_cast<const std::vector<std::pair<float,float>>&, int, int>(&Warp::setWarpMesh), DOC(dai, node, Warp, setWarpMesh))

        .def("setHwIds", &Warp::setHwIds, DOC(dai, node, Warp, setHwIds))
        .def("getHwIds", &Warp::getHwIds, DOC(dai, node, Warp, getHwIds))
        .def("setInterpolation", &Warp::setInterpolation, DOC(dai, node, Warp, setInterpolation))
        .def("getInterpolation", &Warp::getInterpolation, DOC(dai, node, Warp, getInterpolation))
        ;

    daiNodeModule.attr("Warp").attr("Properties") = warpProperties;

}
