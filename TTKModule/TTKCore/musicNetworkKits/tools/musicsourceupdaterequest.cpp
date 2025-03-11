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
    MusicDataSourceRequest *d = new MusicDataSourceRequest(this);
    connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    d->startToRequest(QSyncUtils::makeDataBucketUrl() + QUERY_VERSION_URL);
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
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(bytes, &ok);
        if(!ok)
        {
            return;
        }

        m_rawData = data.toMap();
        Q_EMIT downLoadDataChanged({});
    }
}
