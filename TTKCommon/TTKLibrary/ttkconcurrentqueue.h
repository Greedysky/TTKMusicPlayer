#ifndef TTKCONCURRENTQUEUE_H
#define TTKCONCURRENTQUEUE_H

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

#include <queue>
#include <string.h>
#include <condition_variable>

/*! @brief The class of the concurrent queue.
 * @author Greedysky <greedysky@163.com>
 */
template <typename T>
class TTKConcurrentQueue
{
public:
#ifndef USE_TTK_QUEUE
    /*!
     * Object constructor.
     */
    TTKConcurrentQueue() noexcept
        : m_queue(),
          m_mutex(),
          m_condition()
    {

    }

    /*!
     * Get container data size.
     */
    inline size_t size() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    /*!
     * Check container data is empty or not.
     */
    inline bool empty() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    /*!
     * Clear container data.
     */
    inline void clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::queue<T> empty;
        std::swap(empty, m_queue);
    }

    /*!
     * Push data into container.
     */
    inline void push(const T &value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(value);
        m_condition.notify_one();
    }

    /*!
     * Pop data from container.
     */
    inline bool pop(T &value, bool block = true)
    {
        // If user wants to retrieve data in non-blocking mode
        if(block)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while(m_queue.empty())
            {
                m_condition.wait(lock);
            }
        }
        else
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if(m_queue.empty())
            {
                return false;
            }
        }

        value = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }

private:
    std::queue<T> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_condition;
#else
    /*!
     * Object constructor.
     */
    explicit TTKConcurrentQueue(size_t capacity = 64)
        : m_capacity(capacity),
          m_size(0),
          m_head(0),
          m_tail(0),
          m_buffer(new T[capacity])
    {

    }
    /*!
     * Object destructor.
     */
    ~TTKConcurrentQueue() noexcept
    {
        delete[] m_buffer;
    }

    /*!
     * Get container data size.
     */
    inline size_t size() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_size;
    }

    /*!
     * Check container data is empty or not.
     */
    inline bool empty() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_size == 0;
    }

    /*!
     * Check container data is full or not.
     */
    inline bool full() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_size == m_capacity;
    }

    /*!
     * Get container data capacity size.
     */
    inline size_t capacity() const noexcept
    {
        return m_capacity;
    }

    /*!
     * Clear container data.
     */
    inline void clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_size = m_head = m_tail = 0;
        memset(m_buffer, 0, sizeof(T) * m_capacity);
    }

    /*!
     * Push data into container.
     */
    inline bool push(const T& value, bool block = true)
    {
        if(block)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while(m_size == m_capacity)
            {
                m_full_condition.wait(lock);
            }
        }
        else
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if(m_size == m_capacity)
            {
                return false;
            }
        }

        m_buffer[m_tail] = value;
        m_tail = (m_tail + 1) % m_capacity;
        ++m_size;

        m_empty_condition.notify_one();
        return true;
    }

    /*!
     * Push data into container.
     */
    inline bool push(T&& value, bool block = true)
    {
        if(block)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while(m_size == m_capacity)
            {
                m_full_condition.wait(lock);
            }
        }
        else
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if(m_size == m_capacity)
            {
                return false;
            }
        }

        m_buffer[m_tail] = std::move(value);
        m_tail = (m_tail + 1) % m_capacity;
        ++m_size;

        m_empty_condition.notify_one();
        return true;
    }

    /*!
     * Pop data from container.
     */
    inline bool pop(T& value, bool block = true)
    {
        if(block)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while(m_size == 0)
            {
                m_empty_condition.wait(lock);
            }
        }
        else
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if(m_size == 0)
            {
                return false;
            }
        }

        value = std::move(m_buffer[m_head]);
        m_head = (m_head + 1) % m_capacity;
        --m_size;

        m_full_condition.notify_one();
        return true;
    }

private:
    const size_t m_capacity;
    size_t m_size, m_head, m_tail;
    T* m_buffer;
    std::mutex m_mutex;
    std::condition_variable m_full_condition;
    std::condition_variable m_empty_condition;
#endif
};

#endif // TTKCONCURRENTQUEUE_H
