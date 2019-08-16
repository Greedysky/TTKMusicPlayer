#include "musicdownloadqnconfighread.h"
#include "musicsettingmanager.h"
#///QJson import
#include "qjson/parser.h"

MusicDownloadQNConfighread::MusicDownloadQNConfighread(QObject *parent)
    : MusicNetworkAbstract(parent)
{

}

MusicDownloadQNConfighread::~MusicDownloadQNConfighread()
{
    deleteAll();
}

void MusicDownloadQNConfighread::startToDownload()
{
    m_manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(QUrl(MusicUtils::Algorithm::mdII(CONFIG_QURTY_URL, false)));
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
    MusicObject::setSslConfiguration(&request);
#endif

    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
}

void MusicDownloadQNConfighread::downLoadFinished()
{
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            const QVariantMap &value = data.toMap();
            if(QDateTime::fromString(value["time"].toString(), "yyyy-MM-dd HH:mm:ss").addDays(1) >=
               QDateTime::fromString(M_SETTING_PTR->value(MusicSettingManager::QiNiuTimeConfigChoiced).toString(), "yyyy-MM-dd HH:mm:ss").addMonths(1))
            {
                M_SETTING_PTR->setValue(MusicSettingManager::QiNiuMusicConfigChoiced, value["music_buket"]);
                M_SETTING_PTR->setValue(MusicSettingManager::QiNiuDataConfigChoiced, value["data_buket"]);
                M_SETTING_PTR->setValue(MusicSettingManager::QiNiuTimeConfigChoiced, value["time"]);
            }
        }
    }

    deleteAll();
}
