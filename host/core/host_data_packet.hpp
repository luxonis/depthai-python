#pragma once

#include <assert.h>
#include <string.h>
#include <inttypes.h>

#include <memory>
#include <numeric>
#include <vector>

#include "../../shared/timer.hpp"


#ifdef HOST_PYTHON_MODULE
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;
#endif



struct HostDataPacket
{
    HostDataPacket(
        unsigned size,
        void* in_data,
        const std::string& stream_name_,
        std::vector<int> dimensions_,
        int elem_size_
    )
        : data(size)
        , stream_name(stream_name_)
        , dimensions(dimensions_)
        , elem_size(elem_size_)
    {
        memcpy(data.data(), in_data, size);
        constructor_timer = Timer();
    }

    unsigned size()
    {
        return data.size();
    }

    const unsigned char* getData() const
    {
        return data.data();
    }

#ifdef HOST_PYTHON_MODULE
    py::array* getPythonNumpyArray()
    {
        assert(!dimensions.empty());
        assert(!data.empty());

        Timer t;

        py::array* result = nullptr;

        py::gil_scoped_acquire acquire;

        ssize_t              ndim    = dimensions.size();
        std::vector<ssize_t> shape;
        std::vector<ssize_t> strides;

        auto size_div = std::accumulate(std::begin(dimensions), std::end(dimensions), 1, std::multiplies<int>());
        for (int i = 0; i < dimensions.size(); ++i)
        {
            shape.push_back(dimensions[i]);

            size_div /= dimensions[i];
            strides.push_back(size_div);
        }

        try {
            // TODO: unite code in if blocks
            if (elem_size == 1)
            {
                result = new py::array(py::buffer_info(
                            data.data(),                             /* data as contiguous array  */
                            sizeof(unsigned char),                   /* size of one scalar        */
                            py::format_descriptor<unsigned char>::format(), /* data type          */
                            ndim,                                    /* number of dimensions      */
                            shape,                                   /* shape of the matrix       */
                            strides                                  /* strides for each axis     */
                        ));
            }
            else if (elem_size == 2)
            {
                result = new py::array(py::buffer_info(
                            data.data(),                             /* data as contiguous array  */
                            sizeof(std::uint16_t),                          /* size of one scalar        */
                            py::format_descriptor<std::uint16_t>::format(), /* data type          */
                            2, //ndim,                                    /* number of dimensions      */
                            {720, 1280}, //shape,                                   /* shape of the matrix       */
                            {1280*2, 1*2} //strides                                  /* strides for each axis     */
                        ));
            }
            else
            {
                assert(false);
            }
        } catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            result = new py::array(py::dtype("f"), {1}, {});
        }

        //py::gil_scoped_release release; // REUIRED ???

        // std::cout << "===> c++ getPythonNumpyArray " << t.ellapsed_us() << " us\n";

        return result;
    }
#endif

    std::string getDataAsString()
    {
        assert(data[data.size() - 1] == 0); // checking '\0'
        return reinterpret_cast<const char*>(&data[0]);
    }


    std::vector<unsigned char> data;
    std::string stream_name;
    const std::vector<int> dimensions;
    int elem_size;

    Timer constructor_timer;
};
