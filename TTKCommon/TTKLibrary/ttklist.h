#ifndef TTKLIST_H
#define TTKLIST_H

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

#include <deque>
#include "ttkqtglobal.h"

/*! @brief The class of the list modules.
 * @author Greedysky <greedysky@163.com>
 */
template <typename T>
#if !TTK_QT_VERSION_CHECK(6,0,0)
using TTKList = QList<T>;
#else
class TTKList
{
public:
    using iterator = typename std::deque<T>::iterator;
    using const_iterator = typename std::deque<T>::const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

public:
    using _Container = std::deque<T>;

public:
    TTKList() = default;
    inline TTKList(const T &t) noexcept { append(t); }
    inline TTKList(const TTKList &other) noexcept : m_data(other.m_data) { }
    inline TTKList(TTKList &&other) noexcept : m_data(std::move(other.m_data)) { }

    inline TTKList& operator=(const TTKList &other) noexcept
    {
        m_data = other.m_data;
        return *this;
    }

    inline TTKList& operator=(TTKList &&other) noexcept
    {
        swap(other);
        return *this;
    }

    inline void swap(TTKList &other) noexcept { std::swap(m_data, other.m_data); }

    inline bool operator==(const TTKList &other) const { return m_data == other.m_data; }
    inline bool operator!=(const TTKList &other) const { return m_data != other.m_data; }

    inline int size() const noexcept { return static_cast<int>(m_data.size()); }
    inline bool isEmpty() const { return m_data.empty(); }
    inline void clear() { m_data.clear(); }

    inline T& at(int i) { return m_data.at(i); }
    inline const T& at(int i) const{ return m_data.at(i); }
    inline T& operator[](int i) { return m_data[i]; }
    inline const T& operator[](int i) const { return m_data[i]; }

    inline void append(const T &t) { m_data.push_back(t); }
    inline void append(const TTKList &t) { m_data.insert(m_data.end(), std::make_move_iterator(t.begin()), std::make_move_iterator(t.end())); }
    inline void prepend(const T &t) { m_data.push_front(t); }

    inline void insert(int i, const T &t)
    {
        if(i < 0 || i > static_cast<int>(m_data.size()))
        {
            throw("index out of range");
        }

        m_data.insert(m_data.begin() + i, t);
    }

    inline void replace(int i, const T &t)
    {
        assert(i >= 0 && i < static_cast<int>(m_data.size()));
        m_data[i] = t;
    }

    inline void removeAt(int i)
    {
        m_data.erase(m_data.begin() + i);
    }

    inline int removeAll(const T &t)
    {
        const auto old = m_data.size();
        m_data.erase(std::remove(m_data.begin(), m_data.end(), t), m_data.end());
        return static_cast<int>(old - m_data.size());
    }

    inline bool removeOne(const T &t)
    {
        const auto it = std::find(m_data.begin(), m_data.end(), t);
        if(it == m_data.end())
        {
            return false;
        }

        m_data.erase(it);
        return true;
    }

    inline T takeAt(int i)
    {
        if(i < 0 || i >= static_cast<int>(m_data.size()))
        {
            throw("index out of range");
        }

        const T value = std::move(m_data[i]);
        m_data.erase(m_data.begin() + i);
        return value;
    }

    inline T takeFirst()
    {
        const T value = std::move(m_data.front());
        m_data.pop_front();
        return value;
    }

    inline T takeLast()
    {
        const T value = std::move(m_data.back());
        m_data.pop_back();
        return value;
    }

    inline void move(int from, int to)
    {
        assert(from >= 0 && from < size());
        assert(to >= 0 && to < size());

        if(from == to)
        {
            return;
        }

        const T value = std::move(m_data[from]);

        if(from < to)
        {
            std::move(m_data.begin() + from + 1, m_data.begin() + to + 1, m_data.begin() + from);
        }
        else
        {
            std::move_backward( m_data.begin() + to, m_data.begin() + from, m_data.begin() + from + 1);
        }

        m_data[to] = std::move(value);
    }

    inline void swap(int i, int j)
    {
        assert(i >= 0 && i < size());
        assert(j >= 0 && j < size());

        if(i == j)
        {
            return;
        }

        std::swap(m_data[i], m_data[j]);
    }

    inline int indexOf(const T &t, int from = 0) const
    {
        const int n = size();

        if(from < 0)
        {
            from = std::max(0, n + from);
        }

        if(from >= n)
        {
            return -1;
        }

        for(int i = from; i < n; ++i)
        {
            if(m_data[i] == t)
            {
                return i;
            }
        }
        return -1;
    }

    inline int lastIndexOf(const T &t, int from = -1) const
    {
        const int n = size();

        if(n == 0)
        {
            return -1;
        }

        if(from < 0)
        {
            from += n;
        }

        if(from >= n)
        {
            from = n - 1;
        }

        if(from < 0)
        {
            return -1;
        }

        for(int i = from; i >= 0; --i)
        {
            if(m_data[i] == t)
            {
                return i;
            }
        }
        return -1;
    }

    inline bool contains(const T &t) const { return std::find(m_data.begin(), m_data.end(), t) != m_data.end(); }
    inline int count(const T &t) const { return static_cast<int>(std::count(m_data.begin(), m_data.end(), t)); }

    // stl style
    inline iterator begin() { return m_data.begin(); }
    inline const_iterator begin() const noexcept { return m_data.begin(); }
    inline const_iterator cbegin() const noexcept { return m_data.cbegin(); }
    inline const_iterator constBegin() const noexcept { return m_data.cbegin(); }
    inline iterator end() { return m_data.end(); }
    inline const_iterator end() const noexcept { return m_data.end(); }
    inline const_iterator cend() const noexcept { return m_data.cend(); }
    inline const_iterator constEnd() const noexcept { return m_data.cend(); }
    inline reverse_iterator rbegin() { return m_data.rbegin(); }
    inline reverse_iterator rend() { return m_data.rend(); }
    inline const_reverse_iterator rbegin() const noexcept { return m_data.rbegin(); }
    inline const_reverse_iterator rend() const noexcept { return m_data.rend(); }
    inline const_reverse_iterator crbegin() const noexcept { return m_data.crbegin(); }
    inline const_reverse_iterator crend() const noexcept { return m_data.crend(); }
    inline iterator insert(iterator before, const T &t) { return m_data.insert(before, t); }
    inline iterator erase(iterator pos) { return m_data.erase(pos); }
    inline iterator erase(iterator first, iterator last) { return m_data.erase(first, last); }

    // more Qt
    inline int count() const { return static_cast<int>(m_data.size()); }
    inline int length() const { return static_cast<int>(m_data.size()); } // Same as count()
    inline T& first() { assert(!isEmpty()); return *begin(); }
    inline const T& constFirst() const { return first(); }
    inline const T& first() const { assert(!isEmpty()); return at(0); }
    inline T& last() { assert(!isEmpty()); return *(--end()); }
    inline const T& last() const { assert(!isEmpty()); return at(count() - 1); }
    inline const T& constLast() const { return last(); }
    inline void removeFirst() { assert(!isEmpty()); erase(begin()); }
    inline void removeLast() { assert(!isEmpty()); erase(--end()); }
    inline bool startsWith(const T &t) const { return !isEmpty() && first() == t; }
    inline bool endsWith(const T &t) const { return !isEmpty() && last() == t; }
    inline TTKList mid(int pos, int length = -1) const
    {
        TTKList result;
        const int n = size();

        if(pos < 0 || pos >= n)
        {
            return result;
        }

        if(length < 0)
        {
            length = n - pos;
        }
        else
        {
            length = std::min(length, n - pos);
        }

        for(int i = 0; i < length; ++i)
        {
            result.append(m_data[pos + i]);
        }
        return result;
    }

    inline T value(int i) const { return value(i, T{}); }
    inline T value(int i, const T &defaultValue) const
    {
        if(i < 0 || i >= size())
        {
            return defaultValue;
        }
        return m_data[i];
    }

    // stl compatibility
    inline void push_back(const T &t) { append(t); }
    inline void push_front(const T &t) { prepend(t); }
    inline T& front() { return first(); }
    inline const T& front() const { return first(); }
    inline T& back() { return last(); }
    inline const T& back() const { return last(); }
    inline void pop_front() { removeFirst(); }
    inline void pop_back() { removeLast(); }
    inline bool empty() const { return isEmpty(); }

    // comfort
    inline TTKList& operator+=(const TTKList &other)
    {
        if(this != &other)
        {
            m_data.insert(m_data.end(), other.m_data.begin(), other.m_data.end());
        }
        else
        {
            _Container v(other.m_data);
            m_data.insert(m_data.end(), v.begin(), v.end());
        }
        return *this;
    }
    inline TTKList operator+(const TTKList &other) const { TTKList n = *this; n += other; return n; }
    inline TTKList& operator+=(const T &t) { append(t); return *this; }
    inline TTKList& operator<<(const T &t) { append(t); return *this; }
    inline TTKList& operator<<(const TTKList &other) { *this += other; return *this; }

private:
    _Container m_data;

};
#endif

#endif // TTKLIST_H
