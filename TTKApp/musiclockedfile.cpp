#include "musiclockedfile.h"

MusicLockedFile::MusicLockedFile()
    : QFile()
{
#ifdef Q_OS_WIN
    wmutex = 0;
    rmutex = 0;
#endif
    m_lock_mode = NoLock;
}

MusicLockedFile::MusicLockedFile(const QString &name)
    : QFile(name)
{
#ifdef Q_OS_WIN
    wmutex = 0;
    rmutex = 0;
#endif
    m_lock_mode = NoLock;
}

bool MusicLockedFile::open(OpenMode mode)
{
    if (mode & QIODevice::Truncate) {
        qWarning("MusicLockedFile::open(): Truncate mode not allowed.");
        return false;
    }
    return QFile::open(mode);
}

bool MusicLockedFile::isLocked() const
{
    return m_lock_mode != NoLock;
}

MusicLockedFile::LockMode MusicLockedFile::lockMode() const
{
    return m_lock_mode;
}
