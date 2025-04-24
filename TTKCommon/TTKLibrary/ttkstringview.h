#ifndef TTKSTRINGVIEW_H
#define TTKSTRINGVIEW_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2025 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <string>
#include "ttkmoduleexport.h"

/*! @brief The class of the string view modules.
 * @author Greedysky <greedysky@163.com>
 */
template<typename _CharT, typename _Traits = std::char_traits<_CharT>>
class TTKBaseStringView
{
public:
    using traits_type = _Traits;
    using value_type = std::basic_string<_CharT>;
    using pointer = _CharT*;
    using const_pointer = const _CharT*;
    using reference = _CharT&;
    using const_reference = const _CharT&;
    using const_iterator = const _CharT*;
    using iterator = const_iterator;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using reverse_iterator = const_reverse_iterator;
    using size_type = size_t;
    static constexpr size_type npos = size_type(-1);

public:
    constexpr TTKBaseStringView() noexcept
        : m_data(nullptr),
          m_length(0)
    {

    }

    template<size_type T>
    constexpr TTKBaseStringView(const _CharT(&data)[T])
        : m_data(data),
          m_length(T - 1)
    {

    }

    explicit constexpr TTKBaseStringView(const _CharT *data, const size_type length)
        : m_data(data),
          m_length(length)
    {

    }

    constexpr TTKBaseStringView(const _CharT *data)
        : m_data(data),
          m_length(traits_type::length(data))
    {

    }

    constexpr TTKBaseStringView(const TTKBaseStringView &) noexcept = default;
    TTKBaseStringView(const value_type &s) noexcept
        : m_data(s.data()),
          m_length(s.length())
    {

    }

    constexpr TTKBaseStringView& operator=(const TTKBaseStringView &sv) noexcept
    {
        m_data = sv.data();
        m_length = sv.size();
        return *this;
    }

    explicit constexpr operator value_type() const
    {
        return value_type(m_data, m_length);
    }

    constexpr const_iterator begin() const noexcept { return m_data; }
    constexpr const_iterator end() const noexcept { return m_data + m_length; }
    constexpr const_iterator cbegin() const noexcept { return m_data; }
    constexpr const_iterator cend() const noexcept { return m_data + m_length; }
    constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    constexpr const_reverse_iterator rend() const noexcept{ return const_reverse_iterator(begin()); }
    constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
    constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }
    constexpr size_type size() const noexcept { return m_length; }
    constexpr size_type length() const noexcept { return m_length; }
    constexpr bool empty() const noexcept { return m_length == 0; }
    constexpr const _CharT& operator[](size_type o) const { return m_data[o]; }

    const _CharT& at(size_type o) const
    {
        if(o >= m_length)
        {
            throw std::out_of_range("o exceeds length");
        }
        return m_data[o];
    }

    constexpr const _CharT& front() const noexcept { return m_data[0]; }
    constexpr const _CharT& back() const noexcept { return m_data[m_length - 1]; }
    constexpr const _CharT *data() const noexcept { return m_data; }

    constexpr TTKBaseStringView substr(size_type a) const noexcept
    {
        return a >= m_length ? TTKBaseStringView() : TTKBaseStringView(m_data + a, m_length - a);
    }

    constexpr TTKBaseStringView substr(size_type a, size_type b) const noexcept
    {
        return a >= m_length ? TTKBaseStringView() : TTKBaseStringView(m_data + a, a + b > m_length ? m_length - a : b);
    }

    constexpr size_type find(const _CharT c) const noexcept
    {
        size_type l = 0;
        while(l < m_length)
        {
            if(m_data[l] == c)
            {
                return l;
            }

            ++l;
        }
        return TTKBaseStringView::npos;
    }

    constexpr size_type find(const value_type &c) const noexcept
    {
        size_type l = 0, a = 0;
        while(l < m_length)
        {
            if(m_data[l] != c[a])
            {
                a = 0;
            }

            ++l;
            ++a;

            if(a == c.length())
            {
                return l - c.length();
            }
        }
        return TTKBaseStringView::npos;
    }

    constexpr size_type find(const TTKBaseStringView &sv) const noexcept
    {
        size_type l = 0, a = 0;
        while(l < m_length)
        {
            if(m_data[l] != sv[a])
            {
                a = 0;
            }

            ++l;
            ++a;

            if(a == sv.size())
            {
                return l - sv.size();
            }
        }
        return TTKBaseStringView::npos;
    }

    constexpr size_type find(const _CharT *c) const noexcept
    {
        size_type l = 0, sl = strlen(c), a = 0;
        while(l < m_length)
        {
            if(m_data[l] != c[a])
            {
                a = 0;
            }

            ++l;
            ++a;

            if(a == sl)
            {
                return l - sl;
            }
        }
        return TTKBaseStringView::npos;
    }

    constexpr size_type find(const _CharT *c, size_type sl) const noexcept
    {
        size_type l = 0, a = 0;
        while(l < m_length)
        {
            if(m_data[l] != c[a])
            {
                a = 0;
            }

            ++l;
            ++a;

            if(a == sl)
            {
                return l - sl;
            }
        }
        return TTKBaseStringView::npos;
    }

    constexpr size_type rfind(const _CharT c) const noexcept
    {
        size_type s = m_length;
        while(0 < s)
        {
            if(m_data[s] == c)
            {
                return s;
            }

            --s;
        }
        return TTKBaseStringView::npos;
    }

    constexpr size_type rfind(const _CharT *s) const noexcept
    {
        const size_type m = strlen(s);
        if(m == 1)
        {
            return rfind(*s);
        }

        const size_type n = size();
        if(n < m)
        {
            return TTKBaseStringView::npos;
        }

        size_type c[256] = { 0 };
        for(size_type i = m; i > 0; --i)
        {
            c[int(s[i - 1])] = i;
        }

        for(size_type j = n - m;;)
        {
            if(memcmp(s, m_data + j, m) == 0)
            {
                return j;
            }

            if(j == 0)
            {
                return TTKBaseStringView::npos;
            }

            size_type x = c[int(m_data[j - 1])];
            if(x == 0)
            {
                x = m + 1;
            }

            if(j < x)
            {
                return TTKBaseStringView::npos;
            }

            j -= x;
        }
    }

    constexpr size_type find_first_of(const value_type &s, size_type l = 0) const noexcept
    {
        size_type a = 0;
        while(l < m_length)
        {
            while(s[a])
            {
                if(m_data[l] == s[a])
                {
                    return l;
                }

                ++a;
            }

            ++l;
            a = 0;
        }
        return TTKBaseStringView::npos;
    }

    constexpr size_type find_first_of(const _CharT *s, size_type l = 0) const noexcept
    {
        size_type a = 0;
        while(l < m_length)
        {
            while(s[a])
            {
                if(m_data[l] == s[a])
                {
                    return l;
                }

                ++a;
            }

            ++l;
            a = 0;
        }
        return TTKBaseStringView::npos;
    }

    constexpr size_type find_first_of(_CharT c, size_type l = 0) const noexcept
    {
        while(l < m_length)
        {
            if(m_data[l] == c)
            {
                return l;
            }

            ++l;
        }
        return TTKBaseStringView::npos;
    }

    constexpr size_type find_first_not_of(const value_type &s) const
    {
        if(empty())
        {
            return TTKBaseStringView::npos;
        }

        const size_type r = strspn(data(), s.data());
        return m_data[r] ? r : TTKBaseStringView::npos;
    }

    constexpr size_type find_first_not_of(const value_type &s, size_type o) const
    {
        if(size() <= o)
        {
            return TTKBaseStringView::npos;
        }

        const size_type r = strspn(data() + o, s.data()) + o;
        return m_data[r] ? r : TTKBaseStringView::npos;
    }

    constexpr size_type find_first_not_of(const _CharT *s) const
    {
        if(empty())
        {
            return TTKBaseStringView::npos;
        }

        const size_type r = strspn(this->data(), s);
        return m_data[r] ? r : TTKBaseStringView::npos;
    }

    constexpr size_type find_first_not_of(const _CharT *s, size_type o) const
    {
        if(size() <= o)
        {
            return TTKBaseStringView::npos;
        }

        const size_type r = strspn(this->data() + o, s) + o;
        return m_data[r] ? r : TTKBaseStringView::npos;
    }

    constexpr size_type find_first_not_of(_CharT c, size_type o = 0) const
    {
        if(size() <= o)
        {
            return TTKBaseStringView::npos;
        }

        const _CharT s[2] = { c, '\0' };
        const size_type r = strspn(data() + o, s) + o;
        return m_data[r] ? r : TTKBaseStringView::npos;
    }

    [[nodiscard]] constexpr int compare(const TTKBaseStringView &sv) const noexcept
    {
        const int rc = traits_type::compare(m_data, sv.m_data, (std::min)(m_length, sv.m_length));
        return rc != 0 ? rc : (m_length == sv.m_length ? 0 : m_length < sv.m_length ? -1 : 1);
    }

    [[nodiscard]] constexpr int compare(const _CharT *data) const noexcept
    {
        const size_type l = traits_type::length(data);
        const int rc = traits_type::compare(m_data, data, (std::min)(m_length, l));
        return rc != 0 ? rc : (m_length == l ? 0 : m_length < l ? -1 : 1);
    }

    [[nodiscard]] constexpr int compare(const value_type &s) const noexcept
    {
        const int rc = traits_type::compare(m_data, s.data(), (std::min)(m_length, s.length()));
        return rc != 0 ? rc : (m_length == s.length() ? 0 : m_length < s.length() ? -1 : 1);
    }

    friend std::basic_ostream<_CharT>& operator<<(std::basic_ostream<_CharT> &os, const TTKBaseStringView &sv)
    {
        os.write(sv.m_data, sv.m_length);
        return os;
    }

    friend value_type& operator+=(value_type &s, const TTKBaseStringView &sv)
    {
        s.append(sv.m_data, sv.m_length);
        return s;
    }

    static TTKBaseStringView split(const _CharT *&cur, const _CharT *line_end, _CharT split_char)
    {
        const _CharT *start = cur;
        while(start < (line_end - 1) && *start == split_char)
        {
            ++start;
        }

        const char *end = start + 1;
        while(end < (line_end - 1) && *end != split_char)
        {
            ++end;
        }

        cur = end + 1;
        return TTKBaseStringView(start, cur - start - (*end == split_char ? 1 : 0));
    }

    inline value_type& operator<<(value_type &s) { s.append(data(), size()); return s; }
    // ==
    inline bool operator==(const TTKBaseStringView &other) noexcept { return compare(other) == 0; }
    inline bool operator==(const value_type &other) noexcept { return compare(other) == 0; }
    inline bool operator==(const _CharT *other) noexcept { return compare(other) == 0; }
    // !=
    inline bool operator!=(const TTKBaseStringView &other) noexcept { return compare(other) != 0; }
    inline bool operator!=(const value_type &other) noexcept { return compare(other) != 0; }
    inline bool operator!=(const _CharT *other) noexcept { return compare(other) != 0; }
    // <=
    inline bool operator<=(const TTKBaseStringView &other) noexcept { return compare(other) <= 0; }
    inline bool operator<=(const value_type &other) noexcept { return compare(other) <= 0; }
    inline bool operator<=(const _CharT *other) noexcept { return compare(other) <= 0; }
    // <
    inline bool operator<(const TTKBaseStringView &other) noexcept { return compare(other) < 0; }
    inline bool operator<(const value_type &other) noexcept { return compare(other) < 0; }
    inline bool operator<(const _CharT *other) noexcept { return compare(other) < 0; }
    // >=
    inline bool operator>=(const TTKBaseStringView &other) noexcept { return compare(other) >= 0; }
    inline bool operator>=(const value_type &other) noexcept { return compare(other) >= 0; }
    inline bool operator>=(const _CharT *other) noexcept { return compare(other) >= 0; }
    // >
    inline bool operator>(const TTKBaseStringView &other) noexcept { return compare(other) > 0; }
    inline bool operator>(const value_type &other) noexcept { return compare(other) > 0; }
    inline bool operator>(const _CharT *other) noexcept { return compare(other) > 0; }

private:
    const_pointer m_data;
    size_type m_length;

};

//
using TTKStringView = TTKBaseStringView<char>;
using TTKWStringView = TTKBaseStringView<wchar_t>;
using TTKU16StringView = TTKBaseStringView<char16_t>;
using TTKU32StringView = TTKBaseStringView<char32_t>;

//
template <>
struct std::hash<TTKStringView>
{
    [[nodiscard]] size_t operator()(const TTKStringView sv) const noexcept
    {
#ifdef _MSVC_LANG
        return std::_Hash_array_representation(sv.data(), sv.size());
#else
        return std::_Hash_impl::hash(sv.data(), sv.size());
#endif
    }
};
//
template <>
struct std::hash<TTKWStringView>
{
    [[nodiscard]] size_t operator()(const TTKWStringView sv) const noexcept
    {
#ifdef _MSVC_LANG
        return std::_Hash_array_representation(sv.data(), sv.size());
#else
        return std::_Hash_impl::hash(sv.data(), sv.size());
#endif
    }
};
//
template <>
struct std::hash<TTKU16StringView>
{
    [[nodiscard]] size_t operator()(const TTKU16StringView sv) const noexcept
    {
#ifdef _MSVC_LANG
        return std::_Hash_array_representation(sv.data(), sv.size());
#else
        return std::_Hash_impl::hash(sv.data(), sv.size());
#endif
    }
};
//
template <>
struct std::hash<TTKU32StringView>
{
    [[nodiscard]] size_t operator()(const TTKU32StringView sv) const noexcept
    {
#ifdef _MSVC_LANG
        return std::_Hash_array_representation(sv.data(), sv.size());
#else
        return std::_Hash_impl::hash(sv.data(), sv.size());
#endif
    }
};


// compatiblity for std string_view
namespace std
{
#if !TTK_HAS_CXX17
using string_view = TTKStringView;
using wstring_view = TTKWStringView;
using u16string_view = TTKU16StringView;
using u32string_view = TTKU32StringView;
#endif
}

#endif // TTKSTRINGVIEW_H
