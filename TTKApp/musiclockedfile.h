#ifndef MUSICLOCKEDFILE_H
#define MUSICLOCKEDFILE_H

#include <QFile>
#ifdef Q_OS_WIN
#include <QVector>
#endif
#include "musicrunglobaldefine.h"

namespace MusicLockedPrivate {
  /*! @brief The class of the music local file.
   * @author Greedysky <greedysky@163.com>
   */
    class MUSIC_RUN_EXPORT MusicLockedFile : public QFile
    {
    public:
        enum LockMode
        {
            NoLock = 0,
            ReadLock,
            WriteLock
        };

        MusicLockedFile();
       /*!
         * Object contsructor.
        */
        explicit MusicLockedFile(const QString &name);
        /*!
         * Object contsructor.
        */
        ~MusicLockedFile();

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
        Qt::HANDLE wmutex;
        Qt::HANDLE rmutex;
        QVector<Qt::HANDLE> rmutexes;
        QString mutexname;

        Qt::HANDLE getMutexHandle(int idx, bool doCreate);
        bool waitMutex(Qt::HANDLE mutex, bool doBlock);

    #endif
        LockMode m_lock_mode;
    };
}
#endif //MUSICLOCKEDFILE_H
