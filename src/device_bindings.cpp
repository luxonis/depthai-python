#include "device_bindings.hpp"

//std
#include <iostream>

//project
#include <pybind11/stl.h>
#include <boost/algorithm/string/replace.hpp>



// Binding for HostDataPacket
namespace py = pybind11;

void init_binding_device(pybind11::module& m){
      
    py::class_<Device>(m, "Device")
        .def(py::init<>())
        .def(py::init<std::string, bool>())
        .def(py::init<std::string, std::string>())
        .def(
            "create_pipeline",
            [](Device& device, py::dict config)
            {
                

                // str(dict) for string representation uses ['] , but JSON requires ["]
                // fast & dirty solution:
                std::string str = py::str(config);
                boost::replace_all(str, "\'", "\"");
                boost::replace_all(str, "None", "null");
                boost::replace_all(str, "True", "true");
                boost::replace_all(str, "False", "false");
                // TODO: make better json serialization

                return device.create_pipeline(str);
            },
            "Function for pipeline creation",
            py::arg("config") = py::dict()
        )
        .def(
            "get_available_streams",
            &Device::get_available_streams,
            "Returns available streams, that possible to retreive from the device."
        );

}



/*

class PyDevice {
public:

    PyDevice(std::string a, std::string b){
        device = std::make_unique<Device>(a, b);
    }
    ~PyDevice(){
        device = nullptr;
    }

    std::unique_ptr<Device> device;


    std::shared_ptr<CNNHostPipeline> create_pipeline(py::dict config){

        // str(dict) for string representation uses ['] , but JSON requires ["]
        // fast & dirty solution:
        std::string str = py::str(config);
        boost::replace_all(str, "\'", "\"");
        boost::replace_all(str, "None", "null");
        boost::replace_all(str, "True", "true");
        boost::replace_all(str, "False", "false");
        // TODO: make better json serialization

        return device->create_pipeline(str);
    }

    
    void create_pipeline(py::dict config){
       std::cout << "CNNHostPipeline creationg" << std::endl;
    }

};
*/