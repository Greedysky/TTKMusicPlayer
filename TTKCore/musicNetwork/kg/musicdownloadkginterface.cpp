#include "musicdownloadkginterface.h"
#include "musiccryptographichash.h"
#include "musicnumberutils.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

void MusicDownLoadKGInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                                          const QString &hash, QNetworkAccessManager *manager)
{
    QByteArray encodedData = QCryptographicHash::hash(QString("%1kgcloud").arg(hash).toUtf8(),
                                                      QCryptographicHash::Md5).toHex().toLower();
    QUrl musicUrl = MusicCryptographicHash::decryptData(KG_SONG_ATTR_URL, URL_KEY).arg(QString(encodedData)).arg(hash);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                            &loop, SLOT(quit()));
    loop.exec();

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(!value.isEmpty() && !value.contains("error"))
        {
            MusicObject::MusicSongAttribute attr;
            attr.m_url = value["url"].toString();
            attr.m_size = MusicUtils::Number::size2Label(value["fileSize"].toInt());
            attr.m_format = value["extName"].toString();
            attr.m_bitrate = value["bitRate"].toInt()/1000;
            info->m_songAttrs.append(attr);
        }
    }
}

void MusicDownLoadKGInterface::readFromMusicSongLrcAndPic(MusicObject::MusicSongInfomation *info,
                                                          const QString &hash, QNetworkAccessManager *manager)
{
    QUrl musicUrl = MusicCryptographicHash::decryptData(KG_SONG_INFO_URL, URL_KEY).arg(hash);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                            &loop, SLOT(quit()));
    loop.exec();

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(!value.isEmpty() && value["error"].toString().isEmpty())
        {
            info->m_artistId = QString::number(value["singerId"].toULongLong());
            info->m_smallPicUrl = value["imgUrl"].toString().replace("{size}", "480");
            info->m_lrcUrl = "http://m.kugou.com/app/i/krc.php?cmd=100&keyword=" + value["songName"].toString() +
            "&hash=" + hash + "&timelength=" + QString::number(value["timeLength"].toInt()) + "000&d=0.38664927426725626";
        }
    }
}
