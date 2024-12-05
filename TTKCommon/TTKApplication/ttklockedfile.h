#ifndef TTKLOCKEDFILE_H
#define TTKLOCKEDFILE_H

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

#include <QFile>
#ifdef Q_OS_WIN
#include <QVector>
#endif
#include "ttkmoduleexport.h"

/*! @brief The namespace of locked private.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTKLockedPrivate {
    /*! @brief The class of the ttk local file.
     * @author Greedysky <greedysky@163.com>
     */
    class TTK_MODULE_EXPORT TTKLockedFile : public QFile
    {
    public:
        enum LockMode
        {
            NoLock = 0,
            ReadLock,
            WriteLock
        };

       /*!
         * Object constructor.
        */
        TTKLockedFile();
        explicit TTKLockedFile(const QString &name);
        /*!
         * Object destructor.
        */
        ~TTKLockedFile();

        bool open(OpenMode mode);
        /*!
         * Open current locked file.
        */
        
        bool lock(LockMode mode, bool block = true);
        /*!
         * Lock current file.
        */
        bool unlock();
        /*!
         * Unlock current file.
        */
        bool isLocked() const;
        /*!
         * Get current lock status.
        */
        LockMode lockMode() const;
        /*!
         * Get current lock file mode.
        */
        
    private:
    #ifdef Q_OS_WIN
        Qt::HANDLE m_wmutex;
        Qt::HANDLE m_rmutex;
        QVector<Qt::HANDLE> m_rmutexes;
        QString m_mutexname;

        Qt::HANDLE getMutexHandle(int idx, bool doCreate);
        bool waitMutex(Qt::HANDLE mutex, bool doBlock);

    #endif
        LockMode m_lock_mode;
    };
}

#endif //TTKLOCKEDFILE_H
