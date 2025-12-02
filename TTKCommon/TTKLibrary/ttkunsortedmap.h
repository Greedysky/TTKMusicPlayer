#ifndef TTKUNSORTEDMAP_H
#define TTKUNSORTEDMAP_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#include <vector>

#ifndef _CONSTEXPR_CONTAINER
#  if _HAS_CXX20
#    define _CONSTEXPR_CONTAINER _CONSTEXPR20_CONTAINER
#  else
#    define _CONSTEXPR_CONTAINER inline
#  endif
#endif


/*! @brief The class of the ttk unsorted map module.
 * @author Greedysky <greedysky@163.com>
 */
template <typename _Key, typename _Value>
class TTKUnsortedMap
{
public:
    using key_type = _Key;
    using mapped_type = _Value;
    using value_type = std::pair<key_type, _Value>;
    using reference = _Value&;
    using const_reference = const _Value&;
    using _Container = std::vector<value_type>;
    using size_type = typename _Container::size_type;

public:
    using iterator = typename _Container::iterator;
    using const_iterator = typename _Container::const_iterator;
    using reverse_iterator = typename _Container::reverse_iterator;
    using const_reverse_iterator = typename _Container::const_reverse_iterator;

public:
    _CONSTEXPR_CONTAINER TTKUnsortedMap() noexcept : m_conatiner() { }
    _CONSTEXPR_CONTAINER TTKUnsortedMap(const TTKUnsortedMap &right) noexcept : m_conatiner(right.m_conatiner) { }
    _CONSTEXPR_CONTAINER TTKUnsortedMap(TTKUnsortedMap &&right) noexcept : m_conatiner(std::move(right.m_conatiner)) { }

    template <class _Iter>
    _CONSTEXPR_CONTAINER TTKUnsortedMap(_Iter first, _Iter last) noexcept : TTKUnsortedMap()
    {
        insert(first, last);
    }

    _CONSTEXPR_CONTAINER TTKUnsortedMap& operator=(const TTKUnsortedMap &right) noexcept
    {
        m_conatiner = right.m_conatiner;
        return *this;
    }

    _CONSTEXPR_CONTAINER TTKUnsortedMap& operator=(TTKUnsortedMap &&right) noexcept
    {
        m_conatiner = std::move(right.m_conatiner);
        return *this;
    }

    _CONSTEXPR_CONTAINER void swap(TTKUnsortedMap &right) noexcept
    {
        m_conatiner.swap(right.m_conatiner);
    }

public:
    _CONSTEXPR_CONTAINER void clear() noexcept
    {
        m_conatiner.clear();
    }

    _CONSTEXPR_CONTAINER bool empty() const noexcept
    {
        return m_conatiner.empty();
    }

    _CONSTEXPR_CONTAINER size_type size() const noexcept
    {
        return m_conatiner.size();
    }

    template <class _Iter = iterator, typename std::enable_if<!std::is_same<_Iter, const_iterator>::value, int>::type = 0>
    _CONSTEXPR_CONTAINER iterator erase(iterator where)
    {
        return m_conatiner.erase(where);
    }

    _CONSTEXPR_CONTAINER iterator erase(const_iterator where)
    {
        return m_conatiner.erase(where);
    }

    _CONSTEXPR_CONTAINER iterator erase(const_iterator first, const_iterator last)
    {
        return m_conatiner.erase(first, last);
    }

    _CONSTEXPR_CONTAINER size_type erase(const key_type &key)
    {
        const auto &iter = find(key);
        if(iter == end())
        {
            return 0;
        }

        erase(iter);
        return 1;
    }

public:
    _CONSTEXPR_CONTAINER iterator begin() noexcept
    {
        return m_conatiner.begin();
    }

    _CONSTEXPR_CONTAINER const_iterator begin() const noexcept
    {
        return m_conatiner.begin();
    }

    _CONSTEXPR_CONTAINER iterator end() noexcept
    {
        return m_conatiner.end();
    }

    _CONSTEXPR_CONTAINER const_iterator end() const noexcept
    {
        return m_conatiner.end();
    }

    _CONSTEXPR_CONTAINER reverse_iterator rbegin() noexcept
    {
        return m_conatiner.rbegin();
    }

    _CONSTEXPR_CONTAINER const_reverse_iterator rbegin() const noexcept
    {
        return m_conatiner.rbegin();
    }

    _CONSTEXPR_CONTAINER reverse_iterator rend() noexcept
    {
        return m_conatiner.rend();
    }

    _CONSTEXPR_CONTAINER const_reverse_iterator rend() const noexcept
    {
        return m_conatiner.rend();
    }

    _CONSTEXPR_CONTAINER const_iterator cbegin() const noexcept
    {
        return m_conatiner.cbegin();
    }

    _CONSTEXPR_CONTAINER const_iterator cend() const noexcept
    {
        return m_conatiner.cend();
    }

    _CONSTEXPR_CONTAINER const_reverse_iterator crbegin() const noexcept
    {
        return m_conatiner.crbegin();
    }

    _CONSTEXPR_CONTAINER const_reverse_iterator crend() const noexcept
    {
        return m_conatiner.crend();
    }

public:
    _CONSTEXPR_CONTAINER reference operator[](const key_type &key)
    {
        for(auto &&item : m_conatiner)
        {
            if(item.first == key)
            {
                return item.second;
            }
        }

        m_conatiner.emplace_back(std::move(std::make_pair(key, mapped_type())));
        return m_conatiner.back().second;
    }

    _CONSTEXPR_CONTAINER reference at(const key_type &key)
    {
        return const_cast<reference>(static_cast<const TTKUnsortedMap*>(this)->at(key));
    }

    _CONSTEXPR_CONTAINER const_reference at(const key_type &key) const
    {
        for(auto &&item : m_conatiner)
        {
            if(item.first == key)
            {
                return item.second;
            }
        }

        throw("invalid map<K, T> key");
        return m_conatiner.end()->second;
    }

    _CONSTEXPR_CONTAINER iterator find(const key_type &key) noexcept
    {
        for(auto itr = begin(); itr != end(); ++itr)
        {
            if(itr->first == key)
            {
                return itr;
            }
        }

        return end();
    }

    _CONSTEXPR_CONTAINER const_iterator find(const key_type &key) const noexcept
    {
        for(auto itr = begin(); itr != end(); ++itr)
        {
            if(itr->first == key)
            {
                return itr;
            }
        }

        return end();
    }

    _CONSTEXPR_CONTAINER size_type count(const key_type &key) const noexcept
    {
        return find(key) != end() ? 1 : 0;
    }

#if _HAS_CXX20
    _CONSTEXPR_CONTAINER bool contains(const key_type &key) const noexcept
    {
        return find(key) != end();
    }
#endif

    _CONSTEXPR_CONTAINER iterator lower_bound(const key_type &key) noexcept
    {
        return find(key);
    }

    _CONSTEXPR_CONTAINER const_iterator lower_bound(const key_type &key) const noexcept
    {
        return find(key);
    }

    _CONSTEXPR_CONTAINER iterator upper_bound(const key_type &key) noexcept
    {
        auto iter = find(key);
        return iter != end() ? (++iter != end() ? iter : end()) : end();
    }

    _CONSTEXPR_CONTAINER const_iterator upper_bound(const key_type &key) const noexcept
    {
        auto iter = find(key);
        return iter != end() ? (++iter != end() ? iter : end()) : end();
    }

    _CONSTEXPR_CONTAINER std::pair<iterator, iterator> equal_range(const key_type &key) noexcept
    {
        const auto &iter = find(key);
        if(iter != end())
        {
            const auto &next = std::next(iter);
            if(next != end())
            {
                return { iter, next };
            }
            else
            {
                return { iter, end() };
            }
        }

        return { end(), end() };
    }

    _CONSTEXPR_CONTAINER std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const noexcept
    {
        const auto &iter = find(key);
        if(iter != end())
        {
            const auto &next = std::next(iter);
            if(next != end())
            {
                return { iter, next };
            }
            else
            {
                return { iter, end() };
            }
        }

        return { end(), end() };
    }

public:
    template <class... _Valtys>
    _CONSTEXPR_CONTAINER std::pair<iterator, bool> emplace(_Valtys &&...values)
    {
        _Container _container;
        _container.emplace_back(std::forward<_Valtys>(values)...);

        const auto &iter = find(_container.begin()->first);
        if(iter != end())
        {
            return { iter, false };
        }

        m_conatiner.emplace_back(std::move(_container.front()));
        return { m_conatiner.end() - 1, true };
    }

    template <class... _Valtys>
    _CONSTEXPR_CONTAINER iterator emplace_hint(const_iterator where, _Valtys &&...values) // insert value_type(_Val...) at where
    {
        _Container _container;
        _container.emplace_back(std::forward<_Valtys>(values)...);

        const auto &iter = find(_container.begin()->first);
        if(iter != end())
        {
            return iter;
        }

        return m_conatiner.emplace(where, std::move(_container.front()));
    }

public:
    _CONSTEXPR_CONTAINER iterator insert(const value_type &value)
    {
        auto iter = find(value.first);
        if(iter != end())
        {
            return iter;
        }

        m_conatiner.emplace_back(value);
        return m_conatiner.end() - 1;
    }

    _CONSTEXPR_CONTAINER iterator insert(value_type &&value)
    {
        auto iter = find(value.first);
        if(iter != end())
        {
            return iter;
        }

        m_conatiner.emplace_back(std::move(value));
        return m_conatiner.end() - 1;
    }

    _CONSTEXPR_CONTAINER iterator insert(const_iterator where, const value_type &value)
    {
        auto iter = find(value.first);
        if(iter != end())
        {
            return iter;
        }

        return m_conatiner.emplace(where, value);
    }

    _CONSTEXPR_CONTAINER iterator insert(const_iterator where, value_type &&value)
    {
        auto iter = find(value.first);
        if(iter != end())
        {
            return iter;
        }

        return m_conatiner.emplace(where, std::move(value));
    }

    template <class _Iter>
    _CONSTEXPR_CONTAINER void insert(_Iter first, _Iter last)
    {
        _Adl_verify_range(first, last);
        auto _UFirst = _Get_unwrapped(first);
        const auto _ULast = _Get_unwrapped(last);

        for(; _UFirst != _ULast; ++_UFirst)
        {
            insert(*_UFirst);
        }
    }

    _CONSTEXPR_CONTAINER void insert(std::initializer_list<value_type> list)
    {
        insert(list.begin(), list.end());
    }

private:
    _Container m_conatiner;

};


// compatiblity for std unsorted_map
namespace std
{
// Non-member functions [unsorted_map.nonmembers]
template <class _Key, class _Value>
inline void swap(TTKUnsortedMap<_Key, _Value> &left, TTKUnsortedMap<_Key, _Value> &right) noexcept
{
    left.swap(right);
}

template <class _Key, class _Value>
using unsorted_map = TTKUnsortedMap<_Key, _Value>;
}

#endif // TTKUNSORTEDMAP_H
