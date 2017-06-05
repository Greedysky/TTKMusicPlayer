#include "musicdownloadbdinterface.h"
#include "musiccryptographichash.h"
#include "musicnumberutils.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"
#include "qaeswrap.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

void MusicDownLoadBDInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info, QNetworkAccessManager *manager,
                                                          const QString &bit)
{
    if(!manager)
    {
        return;
    }

    QString key = MusicCryptographicHash::decryptData(BD_SONG_ATTR_PA_URL, URL_KEY).arg(info->m_songId)
                  .arg(MusicTime::timeStamp());
    QString eKey = QString(QAesWrap::encrypt(key.toUtf8(), "4CC20A0C44FEB6FD", "2012061402992850"));
    eKey.replace('+', "%2B");
    eKey.replace('/', "%2F");
    eKey.replace('=', "%3D");
    QUrl musicUrl = MusicCryptographicHash::decryptData(BD_SONG_ATTR_URL, URL_KEY).arg(key).arg(eKey);

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
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["error_code"].toInt() == 22000 && value.contains("songurl"))
        {
            value = value["songurl"].toMap();
            QVariantList datas = value["url"].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                int bitrate = value["file_bitrate"].toInt();
                if(bit.toInt() == bitrate)
                {
                    MusicObject::MusicSongAttribute attr;
                    attr.m_url = value["file_link"].toString();
                    if(attr.m_url.isEmpty())
                    {
                        continue;
                    }

                    if(attr.m_url.contains("pan."))
                    {
                        readFromMusicPayAttribute(info, manager);
                    }
                    else
                    {
                        attr.m_duration = MusicTime::msecTime2LabelJustified(value["file_duration"].toInt()*1000);
                        attr.m_size = MusicUtils::Number::size2Label(value["file_size"].toInt());
                        attr.m_format = value["file_extension"].toString();
                        attr.m_bitrate = map2NormalBitrate(bitrate);
                        info->m_songAttrs.append(attr);
                    }
                }
            }
        }
    }
}

void MusicDownLoadBDInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info, QNetworkAccessManager *manager,
                                                          const QString &format, const QString &quality, bool all)
{
    if(!manager)
    {
        return;
    }

    QString formatString = format;
    foreach(const QString &f, formatString.split(","))
    {
        if(all)
        {
            if(f != "flac")
            {
                readFromMusicSongAttribute(info, manager, f);
            }
            else
            {
                readFromMusicLLAttribute(info, manager);
            }
        }
        else
        {
            if(f != "flac")
            {
                int bit = map2NormalBitrate(f.toInt());
                if(quality == QObject::tr("ST") && bit <= MB_64)
                {
                    readFromMusicSongAttribute(info, manager, f);
                }
                else if(quality == QObject::tr("SD") && bit > MB_64 && bit <= MB_128)
                {
                    readFromMusicSongAttribute(info, manager, f);
                }
                else if(quality == QObject::tr("HQ") && bit > MB_128 && bit <= MB_192)
                {
                    readFromMusicSongAttribute(info, manager, f);
                }
                else if(quality == QObject::tr("SQ") && bit > MB_192 && bit <= MB_320)
                {
                    readFromMusicSongAttribute(info, manager, f);
                }
            }
            else if(quality == QObject::tr("CD"))
            {
                readFromMusicLLAttribute(info, manager);
            }
        }
    }
}

void MusicDownLoadBDInterface::readFromMusicLLAttribute(MusicObject::MusicSongInfomation *info, QNetworkAccessManager *manager)
{
    if(!manager)
    {
        return;
    }

    QUrl musicUrl = MusicCryptographicHash::decryptData(BD_SONG_INFO_URL, URL_KEY).arg(info->m_songId);

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
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["error_code"].toInt() == 22000 && value.contains("data"))
        {
            value = value["data"].toMap();
            QVariantList datas = value["songList"].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                value = value["linkinfo"].toMap();
                if(!value.keys().isEmpty())
                {
                    value = value[value.keys().first()].toMap();
                    MusicObject::MusicSongAttribute attr;
                    attr.m_url = value["songLink"].toString();
                    attr.m_size = MusicUtils::Number::size2Label(value["size"].toInt());
                    attr.m_format = value["format"].toString();
                    attr.m_bitrate = map2NormalBitrate(value["rate"].toInt());
                    info->m_songAttrs.append(attr);
                }
            }
        }
    }
}

void MusicDownLoadBDInterface::readFromMusicPayAttribute(MusicObject::MusicSongInfomation *info, QNetworkAccessManager *manager)
{
    if(!manager)
    {
        return;
    }

    QUrl musicUrl = MusicCryptographicHash::decryptData(BD_SONG_FMINFO_URL, URL_KEY).arg(info->m_songId);

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
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["errorCode"].toInt() == 22000 && value.contains("data"))
        {
            value = value["data"].toMap();
            QVariantList datas = value["songList"].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                MusicObject::MusicSongAttribute attr;
                attr.m_url = value["songLink"].toString();
                attr.m_url.replace(MusicCryptographicHash::decryptData(BD_SONG_YYDOWN_URL, URL_KEY),
                                   MusicCryptographicHash::decryptData(BD_SONG_SSDOWN_URL, URL_KEY));
                attr.m_size = MusicUtils::Number::size2Label(value["size"].toInt());
                attr.m_format = value["format"].toString();
                attr.m_bitrate = map2NormalBitrate(value["rate"].toInt());
                info->m_songAttrs.append(attr);
            }
        }
    }
}

int MusicDownLoadBDInterface::map2NormalBitrate(int bitrate)
{
    if(bitrate > MB_0 && bitrate <= MB_64)
        return MB_32;
    else if(bitrate > MB_64 && bitrate < MB_128)
        return MB_128;
    else if(bitrate > MB_128 && bitrate < MB_192)
        return MB_192;
    else if(bitrate > MB_192 && bitrate < MB_320)
        return MB_320;
    else if(bitrate > MB_320)
        return MB_500;
    else
        return bitrate;
}
