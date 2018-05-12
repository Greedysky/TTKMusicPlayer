#include "ttklockedfile.h"

TTKLockedFile::TTKLockedFile()
    : QFile()
{
#ifdef Q_OS_WIN
    wmutex = 0;
    rmutex = 0;
#endif
    m_lock_mode = NoLock;
}

TTKLockedFile::TTKLockedFile(const QString &name)
    : QFile(name)
{
#ifdef Q_OS_WIN
    wmutex = 0;
    rmutex = 0;
#endif
    m_lock_mode = NoLock;
}

bool TTKLockedFile::open(OpenMode mode)
{
    if (mode & QIODevice::Truncate) {
        qWarning("TTKLockedFile::open(): Truncate mode not allowed.");
        return false;
    }
    return QFile::open(mode);
}

bool TTKLockedFile::isLocked() const
{
    return m_lock_mode != NoLock;
}

TTKLockedFile::LockMode TTKLockedFile::lockMode() const
{
    return m_lock_mode;
}
