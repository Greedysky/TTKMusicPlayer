#include "musicmobiledevicesthread.h"

#ifdef Q_OS_UNIX
#   include <sys/socket.h>
#   include <linux/netlink.h>
#   include <unistd.h>
#endif

MusicMobileDevicesThread::MusicMobileDevicesThread(QObject *parent)
    :   QThread(parent)
{

}

MusicMobileDevicesThread::~MusicMobileDevicesThread()
{
    stopAndQuitThread();
}

void MusicMobileDevicesThread::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
    }
    quit();
}

int MusicMobileDevicesThread::initHotPlugSock()
{
#ifdef Q_OS_UNIX
    const int buffersize = 1024;
    struct sockaddr_nl snl;

    bzero(&snl, sizeof(struct sockaddr_nl));
    snl.nl_family = AF_NETLINK;
    snl.nl_pid = getpid();
    snl.nl_groups = 1;

    int sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if (sock== -1)
    {
        return -1;
    }

    setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize));
    if(bind(sock, (struct sockaddr *)&snl, sizeof(struct sockaddr_nl)) < 0)
    {
        close(sock);
        return -1;
    }
    return sock;
#elif defined Q_OS_WIN
    return -1;
#endif
}

void MusicMobileDevicesThread::start()
{
    m_run = true;
    QThread::start();
}

void MusicMobileDevicesThread::run()
{
    int init = initHotPlugSock();
    if(init == -1)
    {
        return;
    }

#ifdef Q_OS_UNIX
    while(m_run)
    {
        char buf[UEVENT_BUFFER_SIZE * 2] = {0};
        recv(init, &buf, sizeof(buf), 0);
        QString str = buf;

        if(str.indexOf("add@/devices") == 0)
        {
            emit devicesChanged(true);
        }
        if(str.indexOf("remove@/devices") == 0)
        {
            emit devicesChanged(false);
        }
        sleep(1);
    }
#endif
}
