#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "musiclockedfile.h"

bool MusicLockedFile::lock(LockMode mode, bool block)
{
    if (!isOpen()) {
        qWarning("MusicLockedFile::lock(): file is not opened");
        return false;
    }
 
    if (mode == NoLock)
        return unlock();
           
    if (mode == m_lock_mode)
        return true;

    if (m_lock_mode != NoLock)
        unlock();

    struct flock fl;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_type = (mode == ReadLock) ? F_RDLCK : F_WRLCK;
    int cmd = block ? F_SETLKW : F_SETLK;
    int ret = fcntl(handle(), cmd, &fl);
    
    if (ret == -1) {
        if (errno != EINTR && errno != EAGAIN)
            qWarning("MusicLockedFile::lock(): fcntl: %s", strerror(errno));
        return false;
    }

    m_lock_mode = mode;
    return true;
}


bool MusicLockedFile::unlock()
{
    if (!isOpen()) {
        qWarning("MusicLockedFile::unlock(): file is not opened");
        return false;
    }

    if (!isLocked())
        return true;

    struct flock fl;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_type = F_UNLCK;
    int ret = fcntl(handle(), F_SETLKW, &fl);
    
    if (ret == -1) {
        qWarning("MusicLockedFile::lock(): fcntl: %s", strerror(errno));
        return false;
    }
    
    m_lock_mode = NoLock;
    return true;
}

MusicLockedFile::~MusicLockedFile()
{
    if (isOpen())
        unlock();
}

