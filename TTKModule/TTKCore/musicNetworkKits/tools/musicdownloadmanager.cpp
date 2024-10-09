#include "musicdownloadmanager.h"
#include "musicconnectionpool.h"
#include "musicdownloadstatusmodule.h"
#include "musicdownloadrecordwidget.h"
#include "musiccloudtablewidget.h"

void MusicDownLoadManager::connectMultiNetwork(QObject *object)
{
    m_objects << object;
    const QObject *to = G_CONNECTION_PTR->value(MusicDownloadStatusModule::className());
    if(to)
    {
        connect(object, SIGNAL(downLoadDataChanged(QString)), to, SLOT(showDownLoadInfoFinished(QString)));
    }
}

void MusicDownLoadManager::removeMultiNetwork(QObject *object)
{
    const int index = m_objects.indexOf(object);
    if(index != -1)
    {
        m_objects.takeAt(index);
    }
}

void MusicDownLoadManager::connectNetworkData(const MusicDownLoadPairData &pair)
{
    QString className;
    switch(pair.m_type)
    {
        case TTK::Record::NormalDownload: className = MusicDownloadRecordTableWidget::className(); break;
        case TTK::Record::CloudDownload:  className = MusicCloudDownloadTableWidget::className(); break;
        case TTK::Record::CloudUpload:    className = MusicCloudUploadTableWidget::className(); break;
        default: break;
    }

    const QObject *to = G_CONNECTION_PTR->value(className);
    if(to && pair.m_object)
    {
        connect(pair.m_object, SIGNAL(createDownloadItem(QString, qint64)), to, SLOT(createDownloadItem(QString, qint64)));
        connect(pair.m_object, SIGNAL(downloadProgressChanged(float, QString, qint64)), to, SLOT(downloadProgressChanged(float, QString, qint64)));
    }

    connect(pair.m_object, SIGNAL(downloadProgressChanged(float, QString, qint64)), SLOT(downloadProgressChanged(float, QString, qint64)));
    m_datas << pair;
}

void MusicDownLoadManager::reconnectNetworkData(const MusicDownLoadPairData &pair)
{
    const int index = m_datas.indexOf(pair);
    if(index != -1)
    {
        MusicDownLoadPairData *p = &m_datas[index];
        disconnect(p->m_object, SIGNAL(createDownloadItem(QString, qint64)), pair.m_object, SLOT(createDownloadItem(QString, qint64)));
        disconnect(p->m_object, SIGNAL(downloadProgressChanged(float, QString, qint64)), pair.m_object, SLOT(downloadProgressChanged(float, QString, qint64)));

        connect(p->m_object, SIGNAL(createDownloadItem(QString, qint64)), pair.m_object, SLOT(createDownloadItem(QString, qint64)));
        connect(p->m_object, SIGNAL(downloadProgressChanged(float, QString, qint64)), pair.m_object, SLOT(downloadProgressChanged(float, QString, qint64)));
    }
}

void MusicDownLoadManager::removeNetworkData(const MusicDownLoadPairData &pair)
{
    const int index = m_datas.indexOf(pair);
    if(index != -1)
    {
        m_datas.takeAt(index);
    }
}

void MusicDownLoadManager::downloadProgressChanged(float percent, const QString &total, qint64 time)
{
    Q_UNUSED(total);
    if(percent >= 100)
    {
        removeNetworkData(MusicDownLoadPairData(time));
    }
}
