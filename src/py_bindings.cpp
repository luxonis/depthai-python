
#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

// pybind11
#include "pybind11_common.hpp"

// depthai-core
#include "depthai/build/version.hpp"

// project
#include "pipeline/AssetManagerBindings.hpp"
#include "pipeline/PipelineBindings.hpp"
#include "pipeline/NodeBindings.hpp"
#include "XLinkConnectionBindings.hpp"
#include "DeviceBindings.hpp"
#include "DeviceBootloaderBindings.hpp"
#include "DatatypeBindings.hpp"
#include "DataQueueBindings.hpp"
#include "openvino/OpenVINOBindings.hpp"


PYBIND11_MODULE(depthai,m)
{


    //std::string _version = c_depthai_version;
    std::string version = std::string(dai::build::VERSION) + "." + std::string(DEPTHAI_PYTHON_BINDINGS_REVISION);

    #ifdef DEPTHAI_PYTHON_COMMIT_HASH
        version += "+" + std::string(DEPTHAI_PYTHON_COMMIT_HASH);
    #endif

    m.attr("__version__") = version;


    // Add bindings 
    OpenVINOBindings::bind(m);
    AssetManagerBindings::bind(m);
    NodeBindings::bind(m);
    PipelineBindings::bind(m);
    XLinkConnectionBindings::bind(m);
    DeviceBindings::bind(m);
    DeviceBootloaderBindings::bind(m);
    DatatypeBindings::bind(m);
    DataQueueBindings::bind(m);


}



