#include "CommonBindings.hpp"

// depthai-shared
#include "depthai-shared/pb/common/CameraBoardSocket.hpp"

void CommonBindings::bind(pybind11::module& m){

    using namespace dai;
    
    // CameraBoardSocket enum bindings
    py::enum_<CameraBoardSocket>(m, "CameraBoardSocket")
        .value("AUTO", CameraBoardSocket::AUTO)
        .value("RGB", CameraBoardSocket::RGB)
        .value("LEFT", CameraBoardSocket::LEFT)
        .value("RIGHT", CameraBoardSocket::RIGHT)
    ;
    
}