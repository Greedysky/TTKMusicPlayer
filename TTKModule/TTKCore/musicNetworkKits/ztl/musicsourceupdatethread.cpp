#include "musicsourceupdatethread.h"
#include "musicdownloadsourcethread.h"
#include "musicsettingmanager.h"
#///QJson import
#include "qjson/parser.h"
#///Oss import
#include "qoss/ossconf.h"

#define QN_VERSION       "version"

MusicSourceUpdateThread::MusicSourceUpdateThread(QObject *parent)
    : QObject(parent)
{

}

void MusicSourceUpdateThread::startToDownload()
{
    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(OSSConf::generateDataBucketUrl() + QN_VERSION);
}

QString MusicSourceUpdateThread::getVersion() const
{
    return m_rawData["version"].toString();
}

bool MusicSourceUpdateThread::isLastedVersion() const
{
    const QString &v = getVersion();
    if(v.isEmpty())
    {
        return true;
    }
    else
    {
        return v == TTKMUSIC_VERSION_STR;
    }
}

QString MusicSourceUpdateThread::getVersionDescription() const
{
    return m_rawData["data"].toString();
}

void MusicSourceUpdateThread::downLoadFinished(const QByteArray &data)
{
    QJson::Parser parser;
    bool ok;
    const QVariant &parseData = parser.parse(data, &ok);
    if(!ok)
    {
        return;
    }

    m_rawData = parseData.toMap();
    Q_EMIT downLoadDataChanged(m_rawData);
}
