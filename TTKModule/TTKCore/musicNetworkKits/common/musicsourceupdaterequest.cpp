#include "musicsourceupdaterequest.h"
#include "musicdownloadsourcerequest.h"
#include "musicsettingmanager.h"

#include "qsync/qsyncutils.h"

#define OS_VERSION_URL       "version"

MusicSourceUpdateRequest::MusicSourceUpdateRequest(QObject *parent)
    : QObject(parent)
{

}

void MusicSourceUpdateRequest::startToDownload()
{
    MusicDownloadSourceRequest *download = new MusicDownloadSourceRequest(this);
    connect(download, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(QSyncUtils::generateDataBucketUrl() + OS_VERSION_URL);
}

QString MusicSourceUpdateRequest::getVersion() const
{
    return m_rawData["version"].toString();
}

QString MusicSourceUpdateRequest::getVersionDescription() const
{
    return m_rawData["data"].toString();
}

bool MusicSourceUpdateRequest::isLastedVersion() const
{
    const QString &version = getVersion();
    if(version.isEmpty())
    {
        return true;
    }
    else
    {
        return TTKMUSIC_VERSION_STR == version;
    }
}

void MusicSourceUpdateRequest::downLoadFinished(const QByteArray &data)
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
