#include "musicsourceupdaterequest.h"
#include "musicdatasourcerequest.h"

#include "qsync/qsyncutils.h"

static constexpr const char *QUERY_VERSION_URL = "version";

MusicSourceUpdateRequest::MusicSourceUpdateRequest(QObject *parent)
    : QObject(parent)
{

}

void MusicSourceUpdateRequest::startToRequest()
{
    MusicDataSourceRequest *req = new MusicDataSourceRequest(this);
    connect(req, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    req->startToRequest(QSyncUtils::makeDataBucketUrl() + QUERY_VERSION_URL);
}

QString MusicSourceUpdateRequest::version() const noexcept
{
    return m_rawData["version"].toString();
}

QString MusicSourceUpdateRequest::description() const noexcept
{
    return m_rawData["data"].toString();
}

bool MusicSourceUpdateRequest::isLastedVersion() const noexcept
{
    const QString &v = version();
    if(v.isEmpty())
    {
        return true;
    }
    else
    {
        return TTK_VERSION_STR == v;
    }
}

void MusicSourceUpdateRequest::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
    }
    else
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
        if(QJsonParseError::NoError != ok.error)
        {
            return;
        }

        m_rawData = json.toVariant().toMap();
        Q_EMIT downLoadDataChanged({});
    }
}
