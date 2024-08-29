#include "musicpvcounterrequest.h"
#include "ttkabstractxml.h"

#include "qsync/qsyncutils.h"

static constexpr const char *OS_COUNTER_URL = "resource/counter";

MusicPVCounterRequest::MusicPVCounterRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicPVCounterRequest::startToRequest()
{
    QByteArray bytes;
    QFile file(APPCACHE_DIR_FULL + OS_COUNTER_URL);
    if(file.open(QIODevice::ReadOnly))
    {
        TTK_ERROR_STREAM("PV counter using local resource config");
        bytes = file.readAll();
        file.close();
    }
    else
    {
        TTK_ERROR_STREAM("PV counter using network resource config");
        QNetworkRequest request;
        request.setUrl(QSyncUtils::makeDataBucketUrl() + OS_COUNTER_URL);
        bytes = TTK::syncNetworkQueryForGet(&request);
    }

    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Counter PV data error");
        Q_EMIT downLoadDataChanged(TTK_DEFAULT_STR);
        return;
    }

    QNetworkRequest request;
    request.setUrl(QString::fromUtf8(bytes));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicPVCounterRequest::downLoadFinished()
{
    bool ok = false;
    TTK_INFO_STREAM(className() << __FUNCTION__);

    MusicAbstractNetwork::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        TTKAbstractXml xml;
        if(xml.fromByteArray(m_reply->readAll()))
        {
            const QStringList &data = xml.readMultiTextByTagName("tspan");
            if(!data.isEmpty())
            {
                ok = true;
                Q_EMIT downLoadDataChanged(data.join(""));
            }
        }
    }

    if(!ok)
    {
        TTK_ERROR_STREAM("PV counter data error");
        Q_EMIT downLoadDataChanged(TTK_DEFAULT_STR);
    }

    deleteAll();
}
