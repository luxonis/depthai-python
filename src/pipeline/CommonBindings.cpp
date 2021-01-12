#include "CommonBindings.hpp"

// depthai-shared
#include "depthai-shared/pb/common/CameraBoardSocket.hpp"
#include "depthai-shared/pb/common/CameraImageOrientation.hpp"

void CommonBindings::bind(pybind11::module& m){

    using namespace dai;
    
    // CameraBoardSocket enum bindings
    py::enum_<CameraBoardSocket>(m, "CameraBoardSocket")
        .value("AUTO", CameraBoardSocket::AUTO)
        .value("RGB", CameraBoardSocket::RGB)
        .value("LEFT", CameraBoardSocket::LEFT)
        .value("RIGHT", CameraBoardSocket::RIGHT)
    ;
    
    // CameraImageOrientation enum bindings
    py::enum_<CameraImageOrientation>(m, "CameraImageOrientation")
        .value("AUTO", CameraImageOrientation::AUTO)
        .value("NORMAL", CameraImageOrientation::NORMAL)
        .value("HORIZONTAL_MIRROR", CameraImageOrientation::HORIZONTAL_MIRROR)
        .value("VERTICAL_FLIP", CameraImageOrientation::VERTICAL_FLIP)
        .value("ROTATE_180_DEG", CameraImageOrientation::ROTATE_180_DEG)
    ;

}
