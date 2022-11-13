/***************************************************************************
* Copyright (c) Wolf Vollprecht, Johan Mabille and Sylvain Corlay          *
* Copyright (c) QuantStack                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef PYSTRIDES_ADAPTOR_HPP
#define PYSTRIDES_ADAPTOR_HPP

#include <cstddef>
#include <iterator>

namespace xt
{

    template <std::size_t N>
    class pystrides_iterator;

    /*********************************
     * pystrides_adaptor declaration *
     *********************************/

    template <std::size_t N>
    class pystrides_adaptor
    {
    public:

        using value_type = std::ptrdiff_t;
        using const_reference = value_type;
        using reference = const_reference;
        using const_pointer = const value_type*;
        using pointer = const_pointer;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using const_iterator = pystrides_iterator<N>;
        using iterator = const_iterator;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using reverse_iterator = const_reverse_iterator;

        using shape_type = size_t*;

        pystrides_adaptor() = default;
        pystrides_adaptor(const_pointer data, size_type size, shape_type shape);

        bool empty() const noexcept;
        size_type size() const noexcept;

        const_reference operator[](size_type i) const;

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

        const_pointer p_data;
        size_type m_size;
        shape_type p_shape;
    };

    /**********************************
     * pystrides_iterator declaration *
     **********************************/

    template <std::size_t N>
    class pystrides_iterator
    {
    public:

        using self_type = pystrides_iterator<N>;

        using value_type = typename pystrides_adaptor<N>::value_type;
        using pointer = typename pystrides_adaptor<N>::const_pointer;
        using reference = typename pystrides_adaptor<N>::const_reference;
        using difference_type = typename pystrides_adaptor<N>::difference_type;
        using iterator_category = std::random_access_iterator_tag;
        using shape_pointer = typename pystrides_adaptor<N>::shape_type;

        inline pystrides_iterator(pointer current, shape_pointer shape)
            : p_current(current)
            , p_shape(shape)
        {
        }

        inline reference operator*() const
        {
            return *p_shape == size_t(1) ? 0 : *p_current / N;
        }

        inline pointer operator->() const
        {
            // Returning the address of a temporary
            value_type res = this->operator*();
            return &res;
        }

        inline reference operator[](difference_type n) const
        {
            return *(p_current + n) / N;
        }

        inline self_type& operator++()
        {
            ++p_current;
            ++p_shape;
            return *this;
        }

        inline self_type& operator--()
        {
            --p_current;
            --p_shape;
            return *this;
        }

        inline self_type operator++(int)
        {
            self_type tmp(*this);
            ++p_current;
            ++p_shape;
            return tmp;
        }

        inline self_type operator--(int)
        {
            self_type tmp(*this);
            --p_current;
            --p_shape;
            return tmp;
        }

        inline self_type& operator+=(difference_type n)
        {
            p_current += n;
            p_shape += n;
            return *this;
        }

        inline self_type& operator-=(difference_type n)
        {
            p_current -= n;
            p_shape -= n;
            return *this;
        }

        inline self_type operator+(difference_type n) const
        {
            return self_type(p_current + n, p_shape + n);
        }

        inline self_type operator-(difference_type n) const
        {
            return self_type(p_current - n, p_shape - n);
        }

        inline difference_type operator-(const self_type& rhs) const
        {
            self_type tmp(*this);
            return p_current - rhs.p_current;
        }

        pointer get_pointer() const { return p_current; }

    private:

        pointer p_current;
        shape_pointer p_shape;
    };

    template <std::size_t N>
    inline bool operator==(const pystrides_iterator<N>& lhs,
                           const pystrides_iterator<N>& rhs)
    {
        return lhs.get_pointer() == rhs.get_pointer();
    }

    template <std::size_t N>
    inline bool operator!=(const pystrides_iterator<N>& lhs,
                           const pystrides_iterator<N>& rhs)
    {
        return !(lhs == rhs);
    }

    template <std::size_t N>
    inline bool operator<(const pystrides_iterator<N>& lhs,
                          const pystrides_iterator<N>& rhs)
    {
        return lhs.get_pointer() < rhs.get_pointer();
    }

    template <std::size_t N>
    inline bool operator<=(const pystrides_iterator<N>& lhs,
                           const pystrides_iterator<N>& rhs)
    {
        return (lhs < rhs) || (lhs == rhs);
    }

    template <std::size_t N>
    inline bool operator>(const pystrides_iterator<N>& lhs,
                          const pystrides_iterator<N>& rhs)
    {
        return !(lhs <= rhs);
    }

    template <std::size_t N>
    inline bool operator>=(const pystrides_iterator<N>& lhs,
                           const pystrides_iterator<N>& rhs)
    {
        return !(lhs < rhs);
    }

    /************************************
     * pystrides_adaptor implementation *
     ************************************/

    template <std::size_t N>
    inline pystrides_adaptor<N>::pystrides_adaptor(const_pointer data, size_type size, shape_type shape)
        : p_data(data), m_size(size), p_shape(shape)
    {
    }

    template <std::size_t N>
    inline bool pystrides_adaptor<N>::empty() const noexcept
    {
        return m_size == 0;
    }

    template <std::size_t N>
    inline auto pystrides_adaptor<N>::size() const noexcept -> size_type
    {
        return m_size;
    }

    template <std::size_t N>
    inline auto pystrides_adaptor<N>::operator[](size_type i) const -> const_reference
    {
        return p_shape[i] == size_t(1) ? 0 : p_data[i] / N;
    }

    template <std::size_t N>
    inline auto pystrides_adaptor<N>::front() const -> const_reference
    {
        return this->operator[](0);
    }

    template <std::size_t N>
    inline auto pystrides_adaptor<N>::back() const -> const_reference
    {
        return this->operator[](m_size - 1);
    }

    template <std::size_t N>
    inline auto pystrides_adaptor<N>::begin() const -> const_iterator
    {
        return cbegin();
    }

    template <std::size_t N>
    inline auto pystrides_adaptor<N>::end() const -> const_iterator
    {
        return cend();
    }

    template <std::size_t N>
    inline auto pystrides_adaptor<N>::cbegin() const -> const_iterator
    {
        return const_iterator(p_data, p_shape);
    }

    template <std::size_t N>
    inline auto pystrides_adaptor<N>::cend() const -> const_iterator
    {
        return const_iterator(p_data + m_size, p_shape + m_size);
    }

    template <std::size_t N>
    inline auto pystrides_adaptor<N>::rbegin() const -> const_reverse_iterator
    {
        return crbegin();
    }

    template <std::size_t N>
    inline auto pystrides_adaptor<N>::rend() const -> const_reverse_iterator
    {
        return crend();
    }

    template <std::size_t N>
    inline auto pystrides_adaptor<N>::crbegin() const -> const_reverse_iterator
    {
        return const_reverse_iterator(cend());
    }

    template <std::size_t N>
    inline auto pystrides_adaptor<N>::crend() const -> const_reverse_iterator
    {
        return const_reverse_iterator(cbegin());
    }
}

#endif
