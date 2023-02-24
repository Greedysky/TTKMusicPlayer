#ifndef TTKCONCURRENTQUEUE_H
#define TTKCONCURRENTQUEUE_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <queue>
#include <condition_variable>
#include "ttkglobaldefine.h"

/*! @brief The class of the concurrent queue.
 * @author Greedysky <greedysky@163.com>
 */
template <typename T>
class TTK_MODULE_EXPORT TTKConcurrentQueue
{
    TTK_DECLARE_MODULE(TTKConcurrentQueue)
public:
    TTKConcurrentQueue()
        : m_queue(),
          m_mutex(),
          m_condition()
    {

    }

    inline void push(const T &record)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(record);
        m_condition.notify_one();
    }

    inline bool pop(T &record, bool is_blocked = true)
    {
        // If user wants to retrieve data in non-blocking mode
        if(is_blocked)
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

        record = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }

    inline size_t size() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    inline bool empty() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    inline void clear()
    {
        std::queue<T> empty;
        std::swap(empty, m_queue);
    }

private:
  std::queue<T> m_queue;
  mutable std::mutex m_mutex;
  std::condition_variable m_condition;

};

#endif // TTKCONCURRENTQUEUE_H
