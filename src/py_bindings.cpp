
#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

// pybind11

// depthai-core
#include "depthai/Device.hpp"
#include "depthai/build/version.hpp"

// project
//#include "host_data_packet_bindings.hpp"
//#include "nnet_packet_bindings.hpp"
//#include "py_tensor_entry_container_iterator.hpp"
//#include "device_bindings.hpp"

#include "pipeline/PipelineBindings.hpp"
#include "pipeline/NodeBindings.hpp"
#include "XLinkConnectionBindings.hpp"
#include "DeviceBindings.hpp"
#include "DatatypeBindings.hpp"
#include "DataQueueBindings.hpp"


PYBIND11_MODULE(depthai,m)
{


    //std::string _version = c_depthai_version;
    std::string version = std::string(dai::build::VERSION) + "." + std::string(DEPTHAI_PYTHON_BINDINGS_REVISION);

    #ifdef DEPTHAI_PYTHON_COMMIT_HASH
        version += "+" + std::string(DEPTHAI_PYTHON_COMMIT_HASH);
    #endif

    m.attr("__version__") = version;


    // Add bindings 
    NodeBindings::bind(m);
    PipelineBindings::bind(m);
    XLinkConnectionBindings::bind(m);
    DeviceBindings::bind(m);
    DatatypeBindings::bind(m);
    DataQueueBindings::bind(m);


}



