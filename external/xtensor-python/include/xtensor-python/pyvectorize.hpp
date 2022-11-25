/***************************************************************************
* Copyright (c) Wolf Vollprecht, Johan Mabille and Sylvain Corlay          *
* Copyright (c) QuantStack                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef PY_VECTORIZE_HPP
#define PY_VECTORIZE_HPP

#include <type_traits>

#include "pyarray.hpp"
#include "xtensor/xvectorize.hpp"

namespace xt
{

    template <class Func, class R, class... Args>
    struct pyvectorizer
    {
        xvectorizer<Func, R> m_vectorizer;

        template <class F, class = std::enable_if_t<!std::is_same<std::decay_t<F>, pyvectorizer>::value>>
        pyvectorizer(F&& func)
            : m_vectorizer(std::forward<F>(func))
        {
        }

        inline pyarray<R> operator()(const pyarray<Args>&... args) const
        {
            pyarray<R> res = m_vectorizer(args...);
            return res;
        }
    };

    /**
     * @brief Create numpy universal function from scalar function.
     */
    template <class R, class... Args>
    inline pyvectorizer<R (*)(Args...), R, Args...> pyvectorize(R (*f)(Args...))
    {
        return pyvectorizer<R (*)(Args...), R, Args...>(f);
    }

    /// @cond DOXYGEN_INCLUDE_OVERLOADS
    template <class F, class R, class... Args>
    inline pyvectorizer<F, R, Args...> pyvectorize(F&& f, R (*)(Args...))
    {
        return pyvectorizer<F, R, Args...>(std::forward<F>(f));
    }

    template <class F>
    inline auto pyvectorize(F&& f) -> decltype(pyvectorize(std::forward<F>(f), (detail::get_function_type<F>*)nullptr))
    {
        return pyvectorize(std::forward<F>(f), (detail::get_function_type<F>*)nullptr);
    }
    /// @endcond
}

#endif
