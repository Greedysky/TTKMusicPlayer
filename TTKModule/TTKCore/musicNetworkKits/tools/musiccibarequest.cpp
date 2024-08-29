#include "musiccibarequest.h"
#include "musicdatasourcerequest.h"

static constexpr const char *QUERY_URL = "N1ZCWXNTOWkvWEhnTjMxWmk2Y0JNMGZZMzlvTnc0RW1HTCs0bitBbnZzczRWQVcxeWxGaXBhM1QweDQ9";

MusicCiBaRequest::MusicCiBaRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicCiBaRequest::startToRequest()
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(QUERY_URL, false).arg(QDate::currentDate().toString(TTK_DATE_FORMAT)));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

QString MusicCiBaRequest::note() const
{
    return m_rawData["note"].toString();
}

QString MusicCiBaRequest::content() const
{
    return m_rawData["content"].toString();
}

QString MusicCiBaRequest::image() const
{
    return m_rawData["picture4"].toString();
}

void MusicCiBaRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    MusicAbstractNetwork::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            m_rawData = data.toMap();
            TTK_ERROR_STREAM("Download ciba data finish");

            MusicDataSourceRequest *d = new MusicDataSourceRequest(this);
            connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SIGNAL(downLoadRawDataChanged(QByteArray)));
            d->startToRequest(image());
        }
    }

    deleteAll();
}
