#include "XLinkBindings.hpp"

#include "depthai/xlink/XLinkConnection.hpp"
#include "depthai/xlink/XLinkStream.hpp"

#include <cmath>
#include <cstring>

void XLinkBindings::bind(pybind11::module &m, void *pCallstack)
{

    using namespace dai;

    // Type definitions
    py::class_<DeviceInfo> deviceInfo(m, "DeviceInfo", DOC(dai, DeviceInfo));
    py::class_<deviceDesc_t> deviceDesc(m, "DeviceDesc");
    py::enum_<XLinkDeviceState_t> xLinkDeviceState(m, "XLinkDeviceState");
    py::enum_<XLinkProtocol_t> xLinkProtocol(m, "XLinkProtocol");
    py::enum_<XLinkPlatform_t> xLinkPlatform(m, "XLinkPlatform");
    py::class_<XLinkConnection, std::shared_ptr<XLinkConnection> > xLinkConnection(m, "XLinkConnection", DOC(dai, XLinkConnection));

    // pybind11 limitation of having actual classes as exceptions
    // Possible but requires a larger workaround
    // https://stackoverflow.com/questions/62087383/how-can-you-bind-exceptions-with-custom-fields-and-constructors-in-pybind11-and

    // For now just create simple exceptions that will expose what() message
    auto xlinkError = py::register_exception<XLinkError>(m, "XLinkError", PyExc_RuntimeError);
    auto xlinkReadError = py::register_exception<XLinkReadError>(m, "XLinkReadError", xlinkError.ptr());
    auto xlinkWriteError = py::register_exception<XLinkWriteError>(m, "XLinkWriteError", xlinkError.ptr());

    //py::class_<XLinkError> xLinkError(m, "XLinkError", DOC(dai, XLinkError));
    //py::class_<XLinkReadError, XLinkError> xLinkReadError(m, "XLinkReadError", DOC(dai, XLinkReadError));
    //py::class_<XLinkWriteError, XLinkError> xLinkWriteError(m, "XLinkWriteError", DOC(dai, XLinkWriteError));


    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    // Call the rest of the type defines, then perform the actual bindings
    Callstack *callstack = (Callstack *)pCallstack;
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
        .def_property(
            "name",
            [](deviceDesc_t &o)
            { return std::string(o.name); },
            [](deviceDesc_t &o, std::string n)
            { std::strncpy(o.name, n.c_str(), std::min(XLINK_MAX_NAME_SIZE, (int)n.size())); })
        ;

    xLinkDeviceState
        .value("X_LINK_ANY_STATE", X_LINK_ANY_STATE)
        .value("X_LINK_BOOTED", X_LINK_BOOTED)
        .value("X_LINK_UNBOOTED", X_LINK_UNBOOTED)
        .value("X_LINK_BOOTLOADER", X_LINK_BOOTLOADER)
        .value("X_LINK_FLASH_BOOTED", X_LINK_FLASH_BOOTED)
        .export_values()
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
        .def(py::init<const DeviceInfo &, std::vector<std::uint8_t> >())
        .def(py::init<const DeviceInfo &, std::string>())
        .def(py::init<const DeviceInfo &>())
        .def_static("getAllConnectedDevices", &XLinkConnection::getAllConnectedDevices, py::arg("state") = X_LINK_ANY_STATE, py::arg("skipInvalidDevices") = true)
        .def_static("getFirstDevice", &XLinkConnection::getFirstDevice, py::arg("state") = X_LINK_ANY_STATE, py::arg("skipInvalidDevice") = true)
        .def_static("getDeviceByMxId", &XLinkConnection::getDeviceByMxId, py::arg("mxId"), py::arg("state") = X_LINK_ANY_STATE, py::arg("skipInvalidDevice") = true)
        .def_static("bootBootloader", &XLinkConnection::bootBootloader, py::arg("devInfo"))
        ;

    //// Exceptions

    // Applicable if above pybind11 limitation is removed
    // xLinkError
    //     .def(py::init<XLinkError_t, const std::string &, const std::string &>())
    //     .def_readonly("status", &XLinkError::status)
    //     .def_readonly("streamName", &XLinkError::streamName)
    //     .def("what", &XLinkError::what);
    //
    // xLinkReadError
    //     .def(py::init<XLinkError_t, const std::string &>());
    //
    // xLinkWriteError
    //     .def(py::init<XLinkError_t, const std::string &>());


}