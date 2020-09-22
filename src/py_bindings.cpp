#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

// pybind11
#include "pybind11_common.hpp"

#include <boost/algorithm/string/replace.hpp>

// depthai-core
#include "depthai/device.hpp"
#include "depthai/build/version.hpp"

// project
#include "host_data_packet_bindings.hpp"
#include "nnet_packet_bindings.hpp"
#include "device_bindings.hpp"


namespace py = pybind11;
PYBIND11_MODULE(depthai,m)
{

    init_binding_host_data_packet(m);
    init_binding_nnet_packet(m);
    init_binding_device(m);

    // TODO: test ownership in python

    //std::string _version = c_depthai_version;
    std::string version = std::string(dai::build::VERSION) + "." + std::string(DEPTHAI_PYTHON_BINDINGS_REVISION);

    #ifdef DEPTHAI_PYTHON_COMMIT_HASH
        version += "+" + std::string(DEPTHAI_PYTHON_COMMIT_HASH);
    #endif

    m.attr("__version__") = version;


    // TODO
    //std::string _dev_version = c_depthai_dev_version;
    //m.attr("__dev_version__") = "badf00d";

    py::class_<HostPipeline>(m, "Pipeline")
        .def("get_available_data_packets", &HostPipeline::getAvailableDataPackets, py::arg("blocking") = false, py::return_value_policy::copy)
        ;

    py::class_<CNNHostPipeline, std::shared_ptr<CNNHostPipeline>>(m, "CNNPipeline")
        .def("get_available_data_packets", &CNNHostPipeline::getAvailableDataPackets, py::arg("blocking") = false, py::return_value_policy::copy)
        .def("get_available_nnet_and_data_packets", &CNNHostPipeline::getAvailableNNetAndDataPackets, py::arg("blocking") = false, py::return_value_policy::copy)
        ;


    // module destructor
    
    auto cleanup_callback = []() {
        /*
        wdog_stop();
        deinit_device();
        gl_result = nullptr;
        */
    };


    //m.add_object("_cleanup", py::capsule(cleanup_callback));
}



