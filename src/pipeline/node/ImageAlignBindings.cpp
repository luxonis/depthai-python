#include "NodeBindings.hpp"
#include "Common.hpp"

#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/node/ImageAlign.hpp"

void bind_imagealign(pybind11::module &m, void *pCallstack) {

  using namespace dai;
  using namespace dai::node;

  // Node and Properties declare upfront
  py::class_<ImageAlignProperties> properties(m, "ImageAlignProperties",
                                              DOC(dai, ImageAlignProperties));
  auto node = ADD_NODE(ImageAlign);

  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  // Call the rest of the type defines, then perform the actual bindings
  Callstack *callstack = (Callstack *)pCallstack;
  auto cb = callstack->top();
  callstack->pop();
  cb(m, pCallstack);
  // Actual bindings
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////

  // Properties
  properties
      .def_readwrite("initialConfig", &ImageAlignProperties::initialConfig,
                     DOC(dai, ImageAlignProperties, initialConfig))
      .def_readwrite("numFramesPool", &ImageAlignProperties::numFramesPool,
                     DOC(dai, ImageAlignProperties, numFramesPool))
      .def_readwrite("alignWidth", &ImageAlignProperties::alignWidth,
                     DOC(dai, ImageAlignProperties, alignWidth))
      .def_readwrite("alignHeight", &ImageAlignProperties::alignHeight,
                     DOC(dai, ImageAlignProperties, alignHeight))
      .def_readwrite("warpHwIds", &ImageAlignProperties::warpHwIds,
                     DOC(dai, ImageAlignProperties, warpHwIds))
      .def_readwrite("interpolation", &ImageAlignProperties::interpolation,
                     DOC(dai, ImageAlignProperties, interpolation))
      .def_readwrite("outKeepAspectRatio",
                     &ImageAlignProperties::outKeepAspectRatio,
                     DOC(dai, ImageAlignProperties, outKeepAspectRatio))
      .def_readwrite("numShaves", &ImageAlignProperties::numShaves,
                     DOC(dai, ImageAlignProperties, numShaves));
  // Node
  node.def_readonly("inputConfig", &ImageAlign::inputConfig,
                    DOC(dai, node, ImageAlign, inputConfig))
      .def_readonly("input", &ImageAlign::input,
                    DOC(dai, node, ImageAlign, input))
      .def_readonly("inputAlignTo", &ImageAlign::inputAlignTo,
                    DOC(dai, node, ImageAlign, inputAlignTo))
      .def_readonly("outputAligned", &ImageAlign::outputAligned,
                    DOC(dai, node, ImageAlign, outputAligned))
      .def_readonly("passthroughInput", &ImageAlign::passthroughInput,
                    DOC(dai, node, ImageAlign, passthroughInput))
      .def_readonly("initialConfig", &ImageAlign::initialConfig,
                    DOC(dai, node, ImageAlign, initialConfig))
      .def("setOutputSize", &ImageAlign::setOutputSize,
           DOC(dai, node, ImageAlign, setOutputSize))
      .def("setOutKeepAspectRatio", &ImageAlign::setOutKeepAspectRatio,
           DOC(dai, node, ImageAlign, setOutKeepAspectRatio))
      .def("setInterpolation", &ImageAlign::setInterpolation,
           DOC(dai, node, ImageAlign, setInterpolation))
      .def("setNumShaves", &ImageAlign::setNumShaves,
           DOC(dai, node, ImageAlign, setNumShaves))
      .def("setNumFramesPool", &ImageAlign::setNumFramesPool,
           DOC(dai, node, ImageAlign, setNumFramesPool));
  daiNodeModule.attr("ImageAlign").attr("Properties") = properties;
}
