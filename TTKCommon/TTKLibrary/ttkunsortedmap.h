#ifndef TTKUNSORTEDMAP_H
#define TTKUNSORTEDMAP_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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
template <class _Key, class _Value>
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
    _CONSTEXPR_CONTAINER TTKUnsortedMap() noexcept : m_conatiner() {}
    _CONSTEXPR_CONTAINER TTKUnsortedMap(const TTKUnsortedMap &_Right) noexcept : m_conatiner(_Right.m_conatiner) {}
    _CONSTEXPR_CONTAINER TTKUnsortedMap(TTKUnsortedMap &&_Right) noexcept : m_conatiner(std::move(_Right.m_conatiner)) {}

    template <class _Iter>
    _CONSTEXPR_CONTAINER TTKUnsortedMap(_Iter _First, _Iter _Last) noexcept : TTKUnsortedMap()
    {
        insert(_First, _Last);
    }

    _CONSTEXPR_CONTAINER TTKUnsortedMap& operator=(const TTKUnsortedMap &_Right) noexcept
    {
        m_conatiner = _Right.m_conatiner;
        return *this;
    }

    _CONSTEXPR_CONTAINER TTKUnsortedMap& operator=(TTKUnsortedMap &&_Right) noexcept
    {
        m_conatiner = std::move(_Right.m_conatiner);
        return *this;
    }

    _CONSTEXPR_CONTAINER void swap(TTKUnsortedMap &_Right) noexcept
    {
        m_conatiner.swap(_Right.m_conatiner);
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
    _CONSTEXPR_CONTAINER iterator erase(iterator _Where) noexcept
    {
        return m_conatiner.erase(_Where);
    }

    _CONSTEXPR_CONTAINER iterator erase(const_iterator _Where) noexcept
    {
        return m_conatiner.erase(_Where);
    }

    _CONSTEXPR_CONTAINER iterator erase(const_iterator _First, const_iterator _Last) noexcept
    {
        return m_conatiner.erase(_First, _Last);
    }

    _CONSTEXPR_CONTAINER size_type erase(const key_type &_Keyval) noexcept
    {
        const auto &iter = find(_Keyval);
        if (iter == end())
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
    _CONSTEXPR_CONTAINER reference operator[](const key_type &_Keyval)
    {
        for (auto &&item : m_conatiner)
        {
            if (item.first == _Keyval)
            {
                return item.second;
            }
        }

        m_conatiner.emplace_back(std::move(std::make_pair(_Keyval, mapped_type())));
        return m_conatiner.back().second;
    }

    _CONSTEXPR_CONTAINER reference at(const key_type &_Keyval)
    {
        return const_cast<reference>(static_cast<const TTKUnsortedMap*>(this)->at(_Keyval));
    }

    _CONSTEXPR_CONTAINER const_reference at(const key_type &_Keyval) const
    {
        for (auto &&item : m_conatiner)
        {
            if (item.first == _Keyval)
            {
                return item.second;
            }
        }

        throw("invalid map<K, T> key");
        return m_conatiner.end()->second;
    }

    _CONSTEXPR_CONTAINER iterator find(const key_type &_Keyval) noexcept
    {
        for (auto itr = begin(); itr != end(); ++itr)
        {
            if (itr->first == _Keyval)
            {
                return itr;
            }
        }

        return end();
    }

    _CONSTEXPR_CONTAINER const_iterator find(const key_type &_Keyval) const noexcept
    {
        for (auto itr = begin(); itr != end(); ++itr)
        {
            if (itr->first == _Keyval)
            {
                return itr;
            }
        }

        return end();
    }

    _CONSTEXPR_CONTAINER size_type count(const key_type &_Keyval) const noexcept
    {
        return find(_Keyval) != end() ? 1 : 0;
    }

#if _HAS_CXX20
    _CONSTEXPR_CONTAINER bool contains(const key_type &_Keyval) const noexcept
    {
        return find(_Keyval) != end();
    }
#endif

    _CONSTEXPR_CONTAINER iterator lower_bound(const key_type &_Keyval) noexcept
    {
        return find(_Keyval);
    }

    _CONSTEXPR_CONTAINER const_iterator lower_bound(const key_type &_Keyval) const noexcept
    {
        return find(_Keyval);
    }

    _CONSTEXPR_CONTAINER iterator upper_bound(const key_type &_Keyval) noexcept
    {
        auto iter = find(_Keyval);
        return iter != end() ? (++iter != end() ? iter : end()) : end();
    }

    _CONSTEXPR_CONTAINER const_iterator upper_bound(const key_type &_Keyval) const noexcept
    {
        auto iter = find(_Keyval);
        return iter != end() ? (++iter != end() ? iter : end()) : end();
    }

    _CONSTEXPR_CONTAINER std::pair<iterator, iterator> equal_range(const key_type &_Keyval) noexcept
    {
        const auto &iter = find(_Keyval);
        if (iter != end())
        {
            const auto &next = std::next(iter);
            if (next != end())
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

    _CONSTEXPR_CONTAINER std::pair<const_iterator, const_iterator> equal_range(const key_type &_Keyval) const noexcept
    {
        const auto &iter = find(_Keyval);
        if (iter != end())
        {
            const auto &next = std::next(iter);
            if (next != end())
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
    _CONSTEXPR_CONTAINER std::pair<iterator, bool> emplace(_Valtys &&..._Vals)
    {
        _Container _container;
        _container.emplace_back(std::forward<_Valtys>(_Vals)...);

        const auto &iter = find(_container.begin()->first);
        if (iter != end())
        {
            return { iter, false };
        }

        m_conatiner.emplace_back(std::move(_container.front()));
        return { m_conatiner.end() - 1, true };
    }

    template <class... _Valtys>
    _CONSTEXPR_CONTAINER iterator emplace_hint(const_iterator _Where, _Valtys &&..._Vals) // insert value_type(_Val...) at _Where
    {
        _Container _container;
        _container.emplace_back(std::forward<_Valtys>(_Vals)...);

        const auto &iter = find(_container.begin()->first);
        if (iter != end())
        {
            return iter;
        }

        return m_conatiner.emplace(_Where, std::move(_container.front()));
    }

public:
    _CONSTEXPR_CONTAINER iterator insert(const value_type &_Val)
    {
        auto iter = find(_Val.first);
        if (iter != end())
        {
            return iter;
        }

        m_conatiner.emplace_back(_Val);
        return m_conatiner.end() - 1;
    }

    _CONSTEXPR_CONTAINER iterator insert(value_type &&_Val)
    {
        auto iter = find(_Val.first);
        if (iter != end())
        {
            return iter;
        }

        m_conatiner.emplace_back(std::move(_Val));
        return m_conatiner.end() - 1;
    }

    _CONSTEXPR_CONTAINER iterator insert(const_iterator _Where, const value_type &_Val)
    {
        auto iter = find(_Val.first);
        if (iter != end())
        {
            return iter;
        }

        return m_conatiner.emplace(_Where, _Val);
    }

    _CONSTEXPR_CONTAINER iterator insert(const_iterator _Where, value_type &&_Val)
    {
        auto iter = find(_Val.first);
        if (iter != end())
        {
            return iter;
        }

        return m_conatiner.emplace(_Where, std::move(_Val));
    }

    template <class _Iter>
    _CONSTEXPR_CONTAINER void insert(_Iter _First, _Iter _Last)
    {
        _Adl_verify_range(_First, _Last);
        auto _UFirst = _Get_unwrapped(_First);
        const auto _ULast = _Get_unwrapped(_Last);

        for (; _UFirst != _ULast; ++_UFirst)
        {
            insert(*_UFirst);
        }
    }

    _CONSTEXPR_CONTAINER void insert(std::initializer_list<value_type> _Ilist)
    {
        insert(_Ilist.begin(), _Ilist.end());
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
