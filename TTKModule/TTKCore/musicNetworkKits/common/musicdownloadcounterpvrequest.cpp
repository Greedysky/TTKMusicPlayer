#include "musicdownloadcounterpvrequest.h"

MusicDownloadCounterPVRequest::MusicDownloadCounterPVRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

MusicDownloadCounterPVRequest::~MusicDownloadCounterPVRequest()
{
    deleteAll();
}

void MusicDownloadCounterPVRequest::startToDownload()
{
    m_manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(QUrl(MusicUtils::Algorithm::mdII(QURTY_URL, false)));
    request.setRawHeader("Host", MusicUtils::Algorithm::mdII(HOST_URL, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    request.setRawHeader("Cookie", MusicUtils::Algorithm::mdII(COOKIE_URL, false).toUtf8());
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    MusicObject::setSslConfiguration(&request);
#endif

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
}

void MusicDownloadCounterPVRequest::downLoadFinished()
{
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();
        QString value(bytes);
        value.remove("try{BusuanziCallback_850915854978(");
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
            Q_EMIT downLoadDataChanged(QString());
        }
    }
    else
    {
        TTK_LOGGER_ERROR("Counter PV data error");
        Q_EMIT downLoadDataChanged(QString());
    }
    deleteAll();
}
