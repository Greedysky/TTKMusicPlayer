#include "ttklockedfile.h"
#include <qt_windows.h>
#include <QFileInfo>

#define MUTEX_PREFIX "TTKLockedFile mutex "
// Maximum number of concurrent read locks. Must not be greater than MAXIMUM_WAIT_OBJECTS
#define MAX_READERS MAXIMUM_WAIT_OBJECTS

Qt::HANDLE TTKLockedFile::getMutexHandle(int idx, bool doCreate)
{
    if (m_mutexname.isEmpty()) {
        const QFileInfo fin(*this);
        m_mutexname = QString::fromLatin1(MUTEX_PREFIX) + fin.absoluteFilePath().toLower();
    }
    QString mname(m_mutexname);
    if (idx >= 0)
        mname += QString::number(idx);

    Qt::HANDLE mutex;
    if (doCreate) {
        mutex = CreateMutexA(NULL, FALSE, mname.toLocal8Bit().constData());
        if (!mutex) {
            qErrnoWarning("TTKLockedFile::lock(): CreateMutex failed");
            return 0;
        }
    }
    else {
        mutex = OpenMutexA(SYNCHRONIZE | MUTEX_MODIFY_STATE, FALSE, mname.toLocal8Bit().constData());
        if (!mutex) {
            if (GetLastError() != ERROR_FILE_NOT_FOUND)
                qErrnoWarning("TTKLockedFile::lock(): OpenMutex failed");
            return 0;
        }
    }
    return mutex;
}

bool TTKLockedFile::waitMutex(Qt::HANDLE mutex, bool doBlock)
{
    Q_ASSERT(mutex);
    DWORD res = WaitForSingleObject(mutex, doBlock ? INFINITE : 0);
    switch (res) {
    case WAIT_OBJECT_0:
    case WAIT_ABANDONED:
        return true;
        break;
    case WAIT_TIMEOUT:
        break;
    default:
        qErrnoWarning("TTKLockedFile::lock(): WaitForSingleObject failed");
    }
    return false;
}

bool TTKLockedFile::lock(LockMode mode, bool block)
{
    if (!isOpen()) {
        qWarning("TTKLockedFile::lock(): file is not opened");
        return false;
    }

    if (mode == NoLock)
        return unlock();

    if (mode == m_lock_mode)
        return true;

    if (m_lock_mode != NoLock)
        unlock();

    if (!m_wmutex && !(m_wmutex = getMutexHandle(-1, true)))
        return false;

    if (!waitMutex(m_wmutex, block))
        return false;

    if (mode == ReadLock) {
        int idx = 0;
        for (; idx < MAX_READERS; idx++) {
            m_rmutex = getMutexHandle(idx, false);
            if (!m_rmutex || waitMutex(m_rmutex, false))
                break;
            CloseHandle(m_rmutex);
        }
        bool ok = true;
        if (idx >= MAX_READERS) {
            qWarning("TTKLockedFile::lock(): too many readers");
            m_rmutex = 0;
            ok = false;
        }
        else if (!m_rmutex) {
            m_rmutex = getMutexHandle(idx, true);
            if (!m_rmutex || !waitMutex(m_rmutex, false))
                ok = false;
        }
        if (!ok && m_rmutex) {
            CloseHandle(m_rmutex);
            m_rmutex = 0;
        }
        ReleaseMutex(m_wmutex);
        if (!ok)
            return false;
    }
    else {
        Q_ASSERT(m_rmutexes.isEmpty());
        for (int i = 0; i < MAX_READERS; i++) {
            Qt::HANDLE mutex = getMutexHandle(i, false);
            if (mutex)
                m_rmutexes.append(mutex);
        }
        if (!m_rmutexes.isEmpty()) {
            DWORD res = WaitForMultipleObjects(m_rmutexes.count(), m_rmutexes.constData(), TRUE, block ? INFINITE : 0);
            if (res != WAIT_OBJECT_0 && res != WAIT_ABANDONED) {
                if (res != WAIT_TIMEOUT)
                    qErrnoWarning("TTKLockedFile::lock(): WaitForMultipleObjects failed");
                m_lock_mode = WriteLock;  // trick unlock() to clean up - semiyucky
                unlock();
                return false;
            }
        }
    }

    m_lock_mode = mode;
    return true;
}

bool TTKLockedFile::unlock()
{
    if (!isOpen()) {
        qWarning("TTKLockedFile::unlock(): file is not opened");
        return false;
    }

    if (!isLocked())
        return true;

    if (m_lock_mode == ReadLock) {
        ReleaseMutex(m_rmutex);
        CloseHandle(m_rmutex);
        m_rmutex = 0;
    }
    else {
        foreach(Qt::HANDLE mutex, m_rmutexes) {
            ReleaseMutex(mutex);
            CloseHandle(mutex);
        }
        m_rmutexes.clear();
        ReleaseMutex(m_wmutex);
    }

    m_lock_mode = TTKLockedFile::NoLock;
    return true;
}

TTKLockedFile::~TTKLockedFile()
{
    if (isOpen())
        unlock();
    if (m_wmutex)
        CloseHandle(m_wmutex);
}
