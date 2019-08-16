#include "musicdownloadcounterpvthread.h"
#///QJson import
#include "qjson/parser.h"

MusicDownloadCounterPVThread::MusicDownloadCounterPVThread(QObject *parent)
    : MusicNetworkAbstract(parent)
{

}

MusicDownloadCounterPVThread::~MusicDownloadCounterPVThread()
{
    deleteAll();
}

void MusicDownloadCounterPVThread::startToDownload()
{
    m_manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(QUrl(MusicUtils::Algorithm::mdII(QURTY_URL, false)));
    request.setRawHeader("Host", MusicUtils::Algorithm::mdII(HOST_URL, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(REFER_URL, false).toUtf8());
    request.setRawHeader("Cookie", MusicUtils::Algorithm::mdII(COOKIE_URL, false).toUtf8());
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
    MusicObject::setSslConfiguration(&request);
#endif

    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
}

void MusicDownloadCounterPVThread::downLoadFinished()
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
            emit downLoadDataChanged(value["site_pv"].toString());
        }
        else
        {
            emit downLoadDataChanged(QString());
        }
    }
    else
    {
        M_LOGGER_ERROR("Counter PV data error");
        emit downLoadDataChanged(QString());
    }
    deleteAll();
}
