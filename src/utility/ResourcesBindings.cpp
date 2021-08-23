#include "ResourcesBindings.hpp"

// depthai
// include resources #include "depthai/"

void ResourcesBindings::bind(pybind11::module& m, void* pCallstack){

    using namespace dai;

    // Bind Resources (if needed)
    py::class_<Resources, std::unique_ptr<Resources, py::nodelete>>(m, "Resources")
    .def(py::init([](){
        return std::unique_ptr<Resources, py::nodelete>>(&Resources::getInstance());
    });

}



