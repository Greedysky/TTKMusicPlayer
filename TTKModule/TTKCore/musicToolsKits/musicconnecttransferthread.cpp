#include "musicconnecttransferthread.h"

#include <QFileInfo>

MusicConnectTransferThread::MusicConnectTransferThread(QObject *parent)
    : QThread(parent)
{
    m_run = true;
}

MusicConnectTransferThread::~MusicConnectTransferThread()
{
    stopAndQuitThread();
}

void MusicConnectTransferThread::setCopyFilePath(const QString &target, const QStringList &path)
{
    m_target = target;
    m_path << path;
}

void MusicConnectTransferThread::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicConnectTransferThread::run()
{
    foreach(const QString &path, m_path)
    {
        if(m_run && !m_target.isEmpty())
        {
            const QString &targetPath = QString("%1%2").arg(m_target).arg(QFileInfo(path).fileName());
            m_path.takeFirst();
            QFile::copy(path, targetPath);
            emit transferFileFinished(targetPath);
        }
    }
}

void MusicConnectTransferThread::start()
{
    m_run = true;
    QThread::start();
}
