#include "musiccounterpvrequest.h"
#include "ttkabstractxml.h"

#include "qsync/qsyncutils.h"

static constexpr const char *OS_COUNTER_URL = "counter";

MusicCounterPVRequest::MusicCounterPVRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicCounterPVRequest::startToRequest()
{
    QNetworkRequest request;
    request.setUrl(QSyncUtils::makeDataBucketUrl() + OS_COUNTER_URL);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Counter PV data error");
        Q_EMIT downLoadDataChanged(TTK_DEFAULT_STR);
        return;
    }

    request.setUrl(QString::fromUtf8(bytes));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicCounterPVRequest::downLoadFinished()
{
    bool ok = false;

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
        TTK_ERROR_STREAM("Counter PV data error");
        Q_EMIT downLoadDataChanged(TTK_DEFAULT_STR);
    }

    deleteAll();
}
