#include "musicdownloadmginterface.h"
#include "musicnumberutils.h"
#include "musicalgorithmutils.h"
#include "musicsemaphoreloop.h"
#///QJson import
#include "qjson/parser.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

#define MG_INFO_ID         "N2d5bWtRTGJvU0JUbHV2M1hkeHU2MXdFMGwvMW1uZEdIWmpzRkZGZHJ1Tm13K3o4c3A5N1JnPT0="
#define MG_INFO_KEY        "N1Q0a0N0cEZlekJ0MzkvbUIrU2NpenJRUlkvV3krcExvb25VZ1B6N3I5djlqZEhC"
#define MG_INFO_MODE       "NEJqa01QOTE2RUxBcFU4OA=="
#define MG_INFO_CHL        "RGJSMUtjTFVxbTBmKzBxRg=="
#define MG_INFO_RDK        "Nks5TnVjMU5pei9xYk9rTlRNMEcwcUNGZjRseHYvVi9jWXVWNExTUTJDWVdhR0l4MjA4SnZJbXNiYU09"

void MusicDownLoadMGInterface::makeTokenQueryUrl(QNetworkRequest *request, const QString &id)
{
    request->setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request->setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(MG_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    request->setRawHeader("uid", MusicUtils::Algorithm::mdII(QString(MG_INFO_ID), false).toUtf8());
    request->setRawHeader("randomsessionkey", MusicUtils::Algorithm::mdII(QString(MG_INFO_KEY), false).toUtf8());
    request->setRawHeader("mode", MusicUtils::Algorithm::mdII(QString(MG_INFO_MODE), false).toUtf8());
    request->setRawHeader("channel", MusicUtils::Algorithm::mdII(QString(MG_INFO_CHL), false).toUtf8());

    QString time = QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
    request->setRawHeader("TimeStep", time.toUtf8());
    request->setRawHeader("randkey", MusicUtils::Algorithm::md5(MusicUtils::Algorithm::mdII(
                                    QString(MG_INFO_RDK), false).arg(id).arg(time).toUtf8()).toHex());
}

void MusicDownLoadMGInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, int bit,
                                                          const QString &format, int id)
{
    MusicObject::MusicSongAttribute attr;
    attr.m_url = MusicUtils::Algorithm::mdII(MG_SONG_PATH_URL, false).arg(info->m_songId).arg(id);
    attr.m_format = format;
    attr.m_bitrate = bit;
    info->m_songAttrs.append(attr);
}

void MusicDownLoadMGInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info,
                                                          const QVariantMap &format, const QString &quality, bool all)
{
    if(format["control"].toString() == "00000")
    {
        return;
    }

    QString flag = format["toneControl"].toString();
    if(flag.length() < 4)
    {
        return;
    }

    if(all)
    {
        if(flag[0] == '1') readFromMusicSongAttribute(info, MB_128, "mp3", 1);
        if(flag[1] == '1') readFromMusicSongAttribute(info, MB_320, "mp3", 2);
        if(flag[2] == '1') readFromMusicLLAttribute(info);
    }
    else
    {
        if(quality == QObject::tr("SD") && flag[0] == '1')
        {
            readFromMusicSongAttribute(info, MB_128, "mp3", 1);
        }
        else if(quality == QObject::tr("SQ") && flag[1] == '1')
        {
            readFromMusicSongAttribute(info, MB_320, "mp3", 3);
        }
        else if(quality == QObject::tr("CD") && flag[2] == '1')
        {
            readFromMusicLLAttribute(info);
        }
    }
}

void MusicDownLoadMGInterface::readFromMusicLLAttribute(MusicObject::MusicSongInformation *info)
{
    QUrl musicUrl = MusicUtils::Algorithm::mdII(MG_SONG_LL_INFO_URL, false).arg(info->m_songId);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    makeTokenQueryUrl(&request, info->m_songId);
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkAccessManager manager;
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["code"].toString() == "000000" && value.contains("bizs"))
        {
            QVariantList datas = value["bizs"].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                readFromMusicLLAttribute(info, value["params"].toString());
            }
        }
    }
}

void MusicDownLoadMGInterface::readFromMusicLLAttribute(MusicObject::MusicSongInformation *info, const QString &param)
{
    QUrl musicUrl = MusicUtils::Algorithm::mdII(MG_SONG_LL_PATH_URL, false);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    makeTokenQueryUrl(&request, info->m_songId);
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkAccessManager manager;
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager.post(request, MusicUtils::Algorithm::mdII(MG_SONG_LL_DATA_URL, false).arg(info->m_songId).arg(param).toUtf8());
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["code"].toString() == "000000" && value.contains("url"))
        {
            MusicObject::MusicSongAttribute attr;
            attr.m_url = value["url"].toString();
            attr.m_format = value["suffix"].toString();

            attr.m_size = MusicUtils::Number::size2Label(value["size"].toULongLong());
            attr.m_bitrate = MB_1000;
            info->m_songAttrs.append(attr);
        }
    }
}
