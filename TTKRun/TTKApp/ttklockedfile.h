#ifndef TTKLOCKEDFILE_H
#define TTKLOCKEDFILE_H

#include <QFile>
#ifdef Q_OS_WIN
#include <QVector>
#endif
#include "ttkglobaldefine.h"

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

        TTKLockedFile();
       /*!
         * Object contsructor.
        */
        explicit TTKLockedFile(const QString &name);
        /*!
         * Object contsructor.
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
