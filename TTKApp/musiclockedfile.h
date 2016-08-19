#ifndef MUSICLOCKEDFILE_H
#define MUSICLOCKEDFILE_H

#include <QFile>
#ifdef Q_OS_WIN
#include <QVector>
#endif
#include "musicrunglobaldefine.h"

namespace MusicLockedPrivate {
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
        explicit MusicLockedFile(const QString &name);
        ~MusicLockedFile();

        bool open(OpenMode mode);

        bool lock(LockMode mode, bool block = true);
        bool unlock();
        bool isLocked() const;
        LockMode lockMode() const;

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
