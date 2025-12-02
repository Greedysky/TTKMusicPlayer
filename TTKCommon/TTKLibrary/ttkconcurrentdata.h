#ifndef TTKCONCURRENTDATA_H
#define TTKCONCURRENTDATA_H

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

#include <mutex>

/*! @brief The class of the concurrent data.
 * @author Greedysky <greedysky@163.com>
 */
template <typename T>
class TTKConcurrentData
{
public:
    /*!
     * Object constructor.
     */
    TTKConcurrentData() noexcept
        : m_data(),
          m_mutex()
    {

    }

    TTKConcurrentData(const T &v) noexcept
        : m_data(v),
          m_mutex()
    {

    }

    TTKConcurrentData(const TTKConcurrentData &v) noexcept
        : m_data(v.m_data),
          m_mutex(v.m_mutex)
    {

    }

    /*!
     * Set unblocked data.
     */
    bool setUnblockedData(const T &value)
    {
      std::unique_lock<std::mutex> lock(m_mutex);
      if(!lock.try_lock())
      {
        return false;
      }

      m_data = value;
      return true;
    }

    /*!
     * Get unblocked data.
     */
    bool unblockedData(T &value)
    {
      std::unique_lock<std::mutex> lock(m_mutex);
      if(!lock.try_lock())
      {
        return false;
      }

      value = m_data;
      return true;
    }

    /*!
     * Set blocked data.
     */
    void setBlockedData(const T &value)
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_data = value;
    }

    /*!
     * Get blocked data.
     */
    T blockedData() const
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      return m_data;
    }

    /*!
     * Get raw data.
     */
    T* raw()
    {
      return &m_data;
    }

    /*!
     * Get raw mutex.
     */
    std::mutex& mutex()
    {
      return m_mutex;
    }

private:
    T m_data;
    std::mutex m_mutex;

};

#endif // TTKCONCURRENTDATA_H
