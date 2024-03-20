#include "XLinkBindings.hpp"

// std
#include <cmath>
#include <cstring>

// depthai
#include "depthai/xlink/XLinkConnection.hpp"
#include "depthai/xlink/XLinkStream.hpp"


void XLinkBindings::bind(pybind11::module &m, void *pCallstack)
{

    using namespace dai;

    // Type definitions
    py::class_<DeviceInfo> deviceInfo(m, "DeviceInfo", DOC(dai, DeviceInfo));
    py::class_<deviceDesc_t> deviceDesc(m, "DeviceDesc");
    py::enum_<XLinkDeviceState_t> xLinkDeviceState(m, "XLinkDeviceState");
    py::enum_<XLinkProtocol_t> xLinkProtocol(m, "XLinkProtocol");
    py::enum_<XLinkPlatform_t> xLinkPlatform(m, "XLinkPlatform");
    py::enum_<XLinkError_t> xLinkError(m, "XLinkError_t");
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
        .def(py::init<>(), DOC(dai, DeviceInfo, DeviceInfo))
        .def(py::init<std::string, std::string, XLinkDeviceState_t, XLinkProtocol_t, XLinkPlatform_t, XLinkError_t>(), py::arg("name"), py::arg("mxid"), py::arg("state"), py::arg("protocol"), py::arg("platform"), py::arg("status"), DOC(dai, DeviceInfo, DeviceInfo, 2))
        .def(py::init<std::string>(), py::arg("mxidOrName"), DOC(dai, DeviceInfo, DeviceInfo, 3))
        .def(py::init<const deviceDesc_t&>(), DOC(dai, DeviceInfo, DeviceInfo, 4))
        .def("getMxId", &DeviceInfo::getMxId)
        .def("getXLinkDeviceDesc", &DeviceInfo::getXLinkDeviceDesc)
        .def_readwrite("name", &DeviceInfo::name)
        .def_readwrite("mxid", &DeviceInfo::mxid)
        .def_readwrite("state", &DeviceInfo::state)
        .def_readwrite("protocol", &DeviceInfo::protocol)
        .def_readwrite("platform", &DeviceInfo::platform)
        .def_readwrite("status", &DeviceInfo::status)
        .def("__repr__", &DeviceInfo::toString)
        // deprecated
        .def_property("desc", [](py::object& self) {
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "desc field is deprecated, use name/mxid and others instead.", 1);
            return self;
        },[](DeviceInfo& i, DeviceInfo di){
            // Issue an deprecation warning
            PyErr_WarnEx(PyExc_DeprecationWarning, "desc field is deprecated, use name/mxid and others instead.", 1);
            i = di;
        })
        ;

    deviceDesc
        .def(py::init<>())
        .def_readwrite("protocol", &deviceDesc_t::protocol)
        .def_readwrite("platform", &deviceDesc_t::platform)
        .def_readwrite("state", &deviceDesc_t::state)
        .def_readwrite("status", &deviceDesc_t::status)
        .def_property(
            "name",
            [](deviceDesc_t &o)
            { return std::string(o.name); },
            [](deviceDesc_t &o, std::string n)
            {
                memset(o.name, 0, sizeof(o.name));
                std::strncpy(o.name, n.c_str(), sizeof(o.name));
            })
        .def_property(
            "mxid",
            [](deviceDesc_t &o)
            { return std::string(o.mxid); },
            [](deviceDesc_t &o, std::string n)
            {
                memset(o.mxid, 0, sizeof(o.mxid));
                std::strncpy(o.mxid, n.c_str(), sizeof(o.mxid));
            })
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
        .def_static("getGlobalProfilingData", &XLinkConnection::getGlobalProfilingData, DOC(dai, XLinkConnection, getGlobalProfilingData))
        ;

    xLinkError
        .value("X_LINK_SUCCESS", X_LINK_SUCCESS)
        .value("X_LINK_ALREADY_OPEN", X_LINK_ALREADY_OPEN)
        .value("X_LINK_COMMUNICATION_NOT_OPEN", X_LINK_COMMUNICATION_NOT_OPEN)
        .value("X_LINK_COMMUNICATION_FAIL", X_LINK_COMMUNICATION_FAIL)
        .value("X_LINK_COMMUNICATION_UNKNOWN_ERROR", X_LINK_COMMUNICATION_UNKNOWN_ERROR)
        .value("X_LINK_DEVICE_NOT_FOUND", X_LINK_DEVICE_NOT_FOUND)
        .value("X_LINK_TIMEOUT", X_LINK_TIMEOUT)
        .value("X_LINK_ERROR", X_LINK_ERROR)
        .value("X_LINK_OUT_OF_MEMORY", X_LINK_OUT_OF_MEMORY)
        .value("X_LINK_INSUFFICIENT_PERMISSIONS", X_LINK_INSUFFICIENT_PERMISSIONS)
        .value("X_LINK_DEVICE_ALREADY_IN_USE", X_LINK_DEVICE_ALREADY_IN_USE)
        .value("X_LINK_NOT_IMPLEMENTED", X_LINK_NOT_IMPLEMENTED)
        .value("X_LINK_INIT_USB_ERROR", X_LINK_INIT_USB_ERROR)
        .value("X_LINK_INIT_TCP_IP_ERROR", X_LINK_INIT_TCP_IP_ERROR)
        .value("X_LINK_INIT_PCIE_ERROR", X_LINK_INIT_PCIE_ERROR)
        .export_values()
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