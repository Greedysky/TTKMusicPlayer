#include "ttklockedfile.h"

TTKLockedFile::TTKLockedFile() noexcept
    : QFile()
{
#ifdef Q_OS_WIN
    m_wmutex = 0;
    m_rmutex = 0;
#endif
    m_lock_mode = NoLock;
}

TTKLockedFile::TTKLockedFile(const QString &name) noexcept
    : QFile(name)
{
#ifdef Q_OS_WIN
    m_wmutex = 0;
    m_rmutex = 0;
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

bool TTKLockedFile::isLocked() const noexcept
{
    return m_lock_mode != NoLock;
}

TTKLockedFile::LockMode TTKLockedFile::lockMode() const noexcept
{
    return m_lock_mode;
}
