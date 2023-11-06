#include "Common.hpp"
#include "NodeBindings.hpp"

#include "depthai-shared/properties/MessageDemuxProperties.hpp"
#include "depthai/pipeline/Node.hpp"
#include "depthai/pipeline/Pipeline.hpp"
#include "depthai/pipeline/node/MessageDemux.hpp"

void bind_messagedemux(pybind11::module &m, void *pCallstack) {

  using namespace dai;
  using namespace dai::node;

  // Node and Properties declare upfront
  py::class_<MessageDemuxProperties> messageDemuxProperties(
      m, "MessageDemuxProperties", DOC(dai, MessageDemuxProperties));
  auto messageDemux = ADD_NODE(MessageDemux);

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

  // Node
  messageDemux
      .def_readonly("outputs", &MessageDemux::outputs,
                    DOC(dai, node, MessageDemux, outputs))
      .def_readonly("input", &MessageDemux::input,
                    DOC(dai, node, MessageDemux, in))
      .def("setDemuxOnlyOnSuccessful", &MessageDemux::setDemuxOnlyOnSuccessful,
           py::arg("demuxOnlyOnSuccessful"),
           DOC(dai, node, Sync, setDemuxOnlyOnSuccessful));
  daiNodeModule.attr("MessageDemux").attr("Properties") =
      messageDemuxProperties;
}
