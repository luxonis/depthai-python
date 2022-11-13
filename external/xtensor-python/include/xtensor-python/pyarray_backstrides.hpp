/***************************************************************************
* Copyright (c) Wolf Vollprecht, Johan Mabille and Sylvain Corlay          *
* Copyright (c) QuantStack                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef PY_ARRAY_BACKSTRIDES_HPP
#define PY_ARRAY_BACKSTRIDES_HPP

#include <cstddef>
#include <iterator>

namespace xt
{

    /**************************
     * pybackstrides_iterator *
     **************************/

    template <class B>
    class pybackstrides_iterator
    {
    public:

        using self_type = pybackstrides_iterator<B>;

        using value_type = typename B::value_type;
        using pointer = const value_type*;
        using reference = value_type;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;

        pybackstrides_iterator(const B* b, std::size_t offset);

        reference operator*() const;
        pointer operator->() const;

        reference operator[](difference_type n) const;

        self_type& operator++();
        self_type& operator--();

        self_type operator++(int);
        self_type operator--(int);

        self_type& operator+=(difference_type n);
        self_type& operator-=(difference_type n);

        self_type operator+(difference_type n) const;
        self_type operator-(difference_type n) const;
        self_type operator-(const self_type& rhs) const;

        std::size_t offset() const;

    private:

        const B* p_b;
        std::size_t m_offset;
    };

    template <class B>
    inline bool operator==(const pybackstrides_iterator<B>& lhs,
                           const pybackstrides_iterator<B>& rhs);

    template <class B>
    inline bool operator!=(const pybackstrides_iterator<B>& lhs,
                           const pybackstrides_iterator<B>& rhs);

    template <class B>
    inline bool operator<(const pybackstrides_iterator<B>& lhs,
                          const pybackstrides_iterator<B>& rhs);

    template <class B>
    inline bool operator<=(const pybackstrides_iterator<B>& lhs,
                           const pybackstrides_iterator<B>& rhs);

    template <class B>
    inline bool operator>(const pybackstrides_iterator<B>& lhs,
                          const pybackstrides_iterator<B>& rhs);

    template <class B>
    inline bool operator>=(const pybackstrides_iterator<B>& lhs,
                           const pybackstrides_iterator<B>& rhs);

    /***********************
     * pyarray_backstrides *
     ***********************/

    template <class A>
    class pyarray_backstrides
    {
    public:

        using self_type = pyarray_backstrides<A>;
        using array_type = A;
        using value_type = typename array_type::size_type;
        using const_reference = value_type;
        using reference = const_reference;
        using const_pointer = const value_type*;
        using pointer = const_pointer;
        using size_type = typename array_type::size_type;
        using difference_type = typename array_type::difference_type;

        using const_iterator = pybackstrides_iterator<self_type>;
        using iterator = const_iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        pyarray_backstrides() = default;
        pyarray_backstrides(const array_type& a);

        bool empty() const;
        size_type size() const;

        value_type operator[](size_type i) const;

        const_reference front() const;
        const_reference back() const;

        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;

        const_reverse_iterator rbegin() const;
        const_reverse_iterator rend() const;
        const_reverse_iterator crbegin() const;
        const_reverse_iterator crend() const;

    private:

        const array_type* p_a;
    };

    /*****************************************
     * pybackstrides_iterator implementation *
     *****************************************/
    
    template <class B>
    inline pybackstrides_iterator<B>::pybackstrides_iterator(const B* b, std::size_t offset)
        : p_b(b), m_offset(offset)
    {
    }

    template <class B>
    inline auto pybackstrides_iterator<B>::operator*() const -> reference
    {
        return p_b->operator[](m_offset);
    }

    template <class B>
    inline auto pybackstrides_iterator<B>::operator->() const -> pointer
    {
        // Returning the address of a temporary
        value_type res = p_b->operator[](m_offset);
        return &res;
    }

    template <class B>
    inline auto pybackstrides_iterator<B>::operator[](difference_type n) const -> reference
    {
        return p_b->operator[](m_offset + n);
    }

    template <class B>
    inline auto pybackstrides_iterator<B>::operator++() -> self_type&
    {
        ++m_offset;
        return *this;
    }

    template <class B>
    inline auto pybackstrides_iterator<B>::operator--() -> self_type&
    {
        --m_offset;
            return *this;
    }

    template <class B>
    inline auto pybackstrides_iterator<B>::operator++(int )-> self_type
    {
        self_type tmp(*this);
        ++m_offset;
        return tmp;
    }

    template <class B>
    inline auto pybackstrides_iterator<B>::operator--(int) -> self_type
    {
        self_type tmp(*this);
        --m_offset;
        return tmp;
    }

    template <class B>
    inline auto pybackstrides_iterator<B>::operator+=(difference_type n) -> self_type&
    {
        m_offset += n;
        return *this;
    }

    template <class B>
    inline auto pybackstrides_iterator<B>::operator-=(difference_type n) -> self_type&
    {
        m_offset -= n;
        return *this;
    }

    template <class B>
    inline auto pybackstrides_iterator<B>::operator+(difference_type n) const -> self_type
    {
        return self_type(p_b, m_offset + n);
    }

    template <class B>
    inline auto pybackstrides_iterator<B>::operator-(difference_type n) const -> self_type
    {
        return self_type(p_b, m_offset - n);
    }

    template <class B>
    inline auto pybackstrides_iterator<B>::operator-(const self_type& rhs) const -> self_type
    {
        self_type tmp(*this);
        tmp -= (m_offset - rhs.m_offset);
        return tmp;
    }

    template <class B>
    inline std::size_t pybackstrides_iterator<B>::offset() const
    {
        return m_offset;
    }

    template <class B>
    inline bool operator==(const pybackstrides_iterator<B>& lhs,
                           const pybackstrides_iterator<B>& rhs)
    {
        return lhs.offset() == rhs.offset();
    }

    template <class B>
    inline bool operator!=(const pybackstrides_iterator<B>& lhs,
                           const pybackstrides_iterator<B>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class B>
    inline bool operator<(const pybackstrides_iterator<B>& lhs,
                          const pybackstrides_iterator<B>& rhs)
    {
        return lhs.offset() < rhs.offset();
    }

    template <class B>
    inline bool operator<=(const pybackstrides_iterator<B>& lhs,
                           const pybackstrides_iterator<B>& rhs)
    {
        return (lhs < rhs) || (lhs == rhs);
    }

    template <class B>
    inline bool operator>(const pybackstrides_iterator<B>& lhs,
                          const pybackstrides_iterator<B>& rhs)
    {
        return !(lhs <= rhs);
    }

    template <class B>
    inline bool operator>=(const pybackstrides_iterator<B>& lhs,
                           const pybackstrides_iterator<B>& rhs)
    {
        return !(lhs < rhs);
    }

    /**************************************
     * pyarray_backstrides implementation *
     **************************************/

    template <class A>
    inline pyarray_backstrides<A>::pyarray_backstrides(const array_type& a)
        : p_a(&a)
    {
    }

    template <class A>
    inline bool pyarray_backstrides<A>::empty() const
    {
        return p_a->dimension() == 0;
    }

    template <class A>
    inline auto pyarray_backstrides<A>::size() const -> size_type
    {
        return p_a->dimension();
    }

    template <class A>
    inline auto pyarray_backstrides<A>::operator[](size_type i) const -> value_type
    {
        value_type sh = p_a->shape()[i];
        value_type res = sh == 1 ? 0 : (sh - 1) * p_a->strides()[i];
        return res;
    }

    template <class A>
    inline auto pyarray_backstrides<A>::front() const -> const_reference
    {
        value_type sh = p_a->shape()[0];
        value_type res = sh == 1 ? 0 : (sh - 1) * p_a->strides()[0];
        return res;
    }

    template <class A>
    inline auto pyarray_backstrides<A>::back() const -> const_reference
    {
        auto index = p_a->size() - 1;
        value_type sh = p_a->shape()[index];
        value_type res = sh == 1 ? 0 : (sh - 1) * p_a->strides()[index];
        return res;
    }

    template <class A>
    inline auto pyarray_backstrides<A>::begin() const -> const_iterator
    {
        return cbegin();
    }

    template <class A>
    inline auto pyarray_backstrides<A>::end() const -> const_iterator
    {
        return cend();
    }

    template <class A>
    inline auto pyarray_backstrides<A>::cbegin() const -> const_iterator
    {
        return const_iterator(this, 0);
    }

    template <class A>
    inline auto pyarray_backstrides<A>::cend() const -> const_iterator
    {
        return const_iterator(this, size());
    }

    template <class A>
    inline auto pyarray_backstrides<A>::rbegin() const -> const_reverse_iterator
    {
        return crbegin();
    }

    template <class A>
    inline auto pyarray_backstrides<A>::rend() const -> const_reverse_iterator
    {
        return crend();
    }

    template <class A>
    inline auto pyarray_backstrides<A>::crbegin() const -> const_reverse_iterator
    {
        return const_reverse_iterator(end());
    }

    template <class A>
    inline auto pyarray_backstrides<A>::crend() const -> const_reverse_iterator
    {
        return const_reverse_iterator(begin());
    }


}

#endif
