#include "musicdownloadcounterpvrequest.h"
#include "musicobject.h"

MusicDownloadCounterPVRequest::MusicDownloadCounterPVRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicDownloadCounterPVRequest::startToDownload()
{
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QUERY_URL, false));
    request.setRawHeader("Host", MusicUtils::Algorithm::mdII(HOST_URL, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    request.setRawHeader("Cookie", MusicUtils::Algorithm::mdII(COOKIE_URL, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
}

void MusicDownloadCounterPVRequest::downLoadFinished()
{
    MusicAbstractNetwork::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();
        QString value(bytes);
        value.remove(MusicUtils::Algorithm::mdII(RM_KEYWORD, false));
        value.remove(");}catch(e){}");

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(value.toUtf8(), &ok);
        if(ok)
        {
            const QVariantMap &value = data.toMap();
            Q_EMIT downLoadDataChanged(value["site_pv"].toString());
        }
        else
        {
            Q_EMIT downLoadDataChanged(STRING_NULL);
        }
    }
    else
    {
        TTK_LOGGER_ERROR("Counter PV data error");
        Q_EMIT downLoadDataChanged(STRING_NULL);
    }

    deleteAll();
}
