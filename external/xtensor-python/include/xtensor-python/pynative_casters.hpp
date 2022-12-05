/***************************************************************************
* Copyright (c) Wolf Vollprecht, Johan Mabille and Sylvain Corlay          *
* Copyright (c) QuantStack                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef PYNATIVE_CASTERS_HPP
#define PYNATIVE_CASTERS_HPP

#include "xtensor_type_caster_base.hpp"

namespace pybind11
{
    namespace detail
    {
        // Type caster for casting xarray to ndarray
        template <class T, xt::layout_type L>
        struct type_caster<xt::xarray<T, L>> : xtensor_type_caster_base<xt::xarray<T, L>>
        {
        };

        // Type caster for casting xt::xtensor to ndarray
        template <class T, std::size_t N, xt::layout_type L>
        struct type_caster<xt::xtensor<T, N, L>> : xtensor_type_caster_base<xt::xtensor<T, N, L>>
        {
        };

        // Type caster for casting xt::xtensor_fixed to ndarray
        template <class T, class FSH, xt::layout_type L>
        struct type_caster<xt::xtensor_fixed<T, FSH, L>> : xtensor_type_caster_base<xt::xtensor_fixed<T, FSH, L>>
        {
        };

        // Type caster for casting xt::xstrided_view to ndarray
        template <class CT, class S, xt::layout_type L, class FST>
        struct type_caster<xt::xstrided_view<CT, S, L, FST>> : xtensor_type_caster_base<xt::xstrided_view<CT, S, L, FST>>
        {
        };

        // Type caster for casting xt::xarray_adaptor to ndarray
        template <class EC, xt::layout_type L, class SC, class Tag>
        struct type_caster<xt::xarray_adaptor<EC, L, SC, Tag>> : xtensor_type_caster_base<xt::xarray_adaptor<EC, L, SC, Tag>>
        {
        };

        // Type caster for casting xt::xtensor_adaptor to ndarray
        template <class EC, std::size_t N, xt::layout_type L, class Tag>
        struct type_caster<xt::xtensor_adaptor<EC, N, L, Tag>> : xtensor_type_caster_base<xt::xtensor_adaptor<EC, N, L, Tag>>
        {
        };
    }
}

#endif
