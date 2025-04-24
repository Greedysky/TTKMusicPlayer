#ifndef TTKSPINLOCK_H
#define TTKSPINLOCK_H

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

#include <thread>
#include "ttkmoduleexport.h"

/*! @brief The class of the spin lock.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKSpinLock
{
public:
    TTKSpinLock() = default;

    void lock() noexcept
    {
        while(m_lock.test_and_set(std::memory_order_acquire))
        {
            // wait for spin lock to unlock
            std::this_thread::yield();
        }
    }

    bool try_lock() noexcept
    {
        return !m_lock.test_and_set(std::memory_order_acquire);
    }

    void unlock() noexcept
    {
        m_lock.clear(std::memory_order_release);
    }

private:
    std::atomic_flag m_lock = ATOMIC_FLAG_INIT;
    TTK_DISABLE_COPY(TTKSpinLock)

};

/*! @brief The class of the spin lock guard.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKSpinLockGuard
{
public:
    TTKSpinLockGuard(TTKSpinLock& lock) noexcept
        : m_lock(lock)
    {
        m_lock.lock();
    }

    ~TTKSpinLockGuard() noexcept
    {
        m_lock.unlock();
    }

private:
    TTKSpinLock &m_lock;
    TTK_DISABLE_COPY(TTKSpinLockGuard)

};


// compatiblity for std spin_lock
namespace std
{
using spin_lock = TTKSpinLock;
using spin_lock_guard = TTKSpinLockGuard;

using spinlock = spin_lock;
using spinlock_guard = spin_lock_guard;
}

#endif // TTKSPINLOCK_H
