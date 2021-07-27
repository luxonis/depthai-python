#include "XLinkConnectionBindings.hpp"

#include "depthai/xlink/XLinkConnection.hpp"

#include <cmath>
#include <cstring>

void XLinkConnectionBindings::bind(pybind11::module& m, void* pCallstack){

    using namespace dai;


    // Type definitions
    py::class_<DeviceInfo> deviceInfo(m, "DeviceInfo", DOC(dai, DeviceInfo));
    py::class_<deviceDesc_t> deviceDesc(m, "DeviceDesc");
    py::enum_<XLinkDeviceState_t> xLinkDeviceState(m, "XLinkDeviceState");
    py::enum_<XLinkProtocol_t> xLinkProtocol(m, "XLinkProtocol");
    py::enum_<XLinkPlatform_t> xLinkPlatform(m, "XLinkPlatform");
    py::class_<XLinkConnection, std::shared_ptr<XLinkConnection>> xLinkConnection(m, "XLinkConnection", DOC(dai, XLinkConnection));


    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    // Call the rest of the type defines, then perform the actual bindings
    Callstack* callstack = (Callstack*) pCallstack;
    auto cb = callstack->top();
    callstack->pop();
    cb(m, pCallstack);
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    // Bindings
    deviceInfo
        .def(py::init<>())
        .def_readwrite("desc", &DeviceInfo::desc)
        .def_readwrite("state", &DeviceInfo::state)
        .def("getMxId", &DeviceInfo::getMxId)
        ;

    deviceDesc
        .def(py::init<>())
        .def_readwrite("protocol", &deviceDesc_t::protocol)
        .def_readwrite("platform", &deviceDesc_t::platform)
        .def_property("name",
            [](deviceDesc_t& o){return std::string(o.name);},
            [](deviceDesc_t& o, std::string n){ std::strncpy(o.name, n.c_str(), std::min(XLINK_MAX_NAME_SIZE,(int) n.size()));}
        )
        ;

    xLinkDeviceState
        .value("X_LINK_ANY_STATE", X_LINK_ANY_STATE)
        .value("X_LINK_BOOTED", X_LINK_BOOTED)
        .value("X_LINK_UNBOOTED", X_LINK_UNBOOTED)
        .value("X_LINK_BOOTLOADER", X_LINK_BOOTLOADER)
        .export_values();
        ;


    xLinkProtocol
        .value("X_LINK_USB_VSC", X_LINK_USB_VSC)
        .value("X_LINK_USB_CDC", X_LINK_USB_CDC)
        .value("X_LINK_PCIE", X_LINK_PCIE)
        .value("X_LINK_TCP_IP", X_LINK_TCP_IP)
        .value("X_LINK_IPC", X_LINK_IPC)
        .value("X_LINK_NMB_OF_PROTOCOLS", X_LINK_NMB_OF_PROTOCOLS)
        .value("X_LINK_ANY_PROTOCOL", X_LINK_ANY_PROTOCOL)
        .export_values()
        ;

    xLinkPlatform
        .value("X_LINK_ANY_PLATFORM", X_LINK_ANY_PLATFORM)
        .value("X_LINK_MYRIAD_2", X_LINK_MYRIAD_2)
        .value("X_LINK_MYRIAD_X", X_LINK_MYRIAD_X)
        .export_values()
        ;

    xLinkConnection
        .def(py::init<const DeviceInfo&, std::vector<std::uint8_t>>())
        .def(py::init<const DeviceInfo&, std::string>())
        .def(py::init<const DeviceInfo&>())
        .def_static("getAllConnectedDevices", &XLinkConnection::getAllConnectedDevices, py::arg("state") = X_LINK_ANY_STATE)
        .def_static("getFirstDevice", &XLinkConnection::getFirstDevice, py::arg("state") = X_LINK_ANY_STATE)
        .def_static("getDeviceByMxId", &XLinkConnection::getDeviceByMxId, py::arg("mxId"), py::arg("state") = X_LINK_ANY_STATE)
        ;


}