#include "musicconnecttransferthread.h"

#include <QFileInfo>

MusicConnectTransferThread::MusicConnectTransferThread(QObject *parent)
    : TTKAbstractThread(parent)
{

}

void MusicConnectTransferThread::setCopyFilePath(const QString &target, const QStringList &path)
{
    m_target = target;
    m_path << path;
}

void MusicConnectTransferThread::run()
{
    TTKAbstractThread::run();

    for(const QString &path : qAsConst(m_path))
    {
        if(m_running && !m_target.isEmpty())
        {
            const QString &targetPath = QString("%1%2").arg(m_target, QFileInfo(path).fileName());
            m_path.takeFirst();
            QFile::copy(path, targetPath);
            Q_EMIT transferFileFinished(targetPath);
        }
    }
}
