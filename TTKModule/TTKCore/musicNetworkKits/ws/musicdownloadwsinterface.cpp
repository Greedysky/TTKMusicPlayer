#include "musicdownloadwsinterface.h"
#include "musicsemaphoreloop.h"
#include "musicnumberutils.h"
#include "musicalgorithmutils.h"
#include "musiccoreutils.h"
#///QJson import
#include "qjson/parser.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

void MusicDownLoadWSInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QString &queryType, int bitrate)
{
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WS_SONG_URL, false).arg(info->m_songId).arg(queryType);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(WS_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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
        if(value["code"].toInt() == 0 && value.contains("data"))
        {
            value = value["data"].toMap();
            if(value.isEmpty())
            {
                return;
            }

            if(info->m_songName == "-")
            {
                info->m_songName = value["SN"].toString();
            }

            MusicObject::MusicSongAttribute attr;
            switch(bitrate)
            {
                case MB_32:
                    {
                        attr.m_bitrate = MB_32;
                        attr.m_url = value["lqurl"].toString();
                        attr.m_size = MusicUtils::Number::size2Label(value["lqsize"].toInt());
                        if(attr.m_url.isEmpty())
                        {
                            value["KLM"].toString();
                        }
                        break;
                    }
                case MB_128:
                    {
                        attr.m_bitrate = MB_128;
                        attr.m_url = value["hqurl"].toString();
                        attr.m_size = MusicUtils::Number::size2Label(value["hqsize"].toInt());
                        if(attr.m_url.isEmpty())
                        {
                            value["KL128"].toString();
                        }
                        break;
                    }
                case MB_192:
                    {
                        attr.m_bitrate = MB_192;
                        attr.m_url = value["squrl"].toString();
                        attr.m_size = MusicUtils::Number::size2Label(value["sqsize"].toInt());
                        if(attr.m_url.isEmpty())
                        {
                            value["KL"].toString();
                        }
                        break;
                    }
                default: return;
            }

            if(attr.m_url.isEmpty() || info->m_songAttrs.contains(attr))
            {
                return;
            }

            info->m_lrcUrl = value["SW"].toString();
            info->m_smallPicUrl = value["SI"].toString();
            attr.m_format = MusicUtils::Core::fileSuffix(attr.m_url);
            info->m_songAttrs.append(attr);
        }
    }
}

void MusicDownLoadWSInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QString &queryType, const QString &quality, bool all)
{
    if(all)
    {
        readFromMusicSongAttribute(info, queryType, MB_32);
        readFromMusicSongAttribute(info, queryType, MB_128);
        readFromMusicSongAttribute(info, queryType, MB_192);
    }
    else
    {
        if(quality == QObject::tr("ST"))
        {
            readFromMusicSongAttribute(info, queryType, MB_32);
        }
        else if(quality == QObject::tr("SD"))
        {
            readFromMusicSongAttribute(info, queryType, MB_128);
        }
        else if(quality == QObject::tr("HQ"))
        {
            readFromMusicSongAttribute(info, queryType, MB_192);
        }
    }
}
