#include "musickgqueryinterface.h"
#include "musicnumberutils.h"
#include "musicsemaphoreloop.h"
#include "musicalgorithmutils.h"
#include "musicabstractqueryrequest.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

void MusicKGQueryInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QString &hash)
{
    if(hash.isEmpty())
    {
        return;
    }

    const QByteArray &encodedData = MusicUtils::Algorithm::md5(QString("%1kgcloudv2").arg(hash).toUtf8()).toHex().toLower();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_SONG_DETAIL_URL, false).arg(hash).arg(QString(encodedData)));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KG_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

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
    const QVariant &data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        const QVariantMap &value = data.toMap();
        if(!value.isEmpty() && !value.contains("error"))
        {
            const int bitRate = value["bitRate"].toInt();
            if(bitRate == 0)
            {
                return;
            }

            MusicObject::MusicSongAttribute attr;
            attr.m_url = value["url"].toString();
            attr.m_size = MusicUtils::Number::size2Label(value["fileSize"].toInt());
            attr.m_format = value["extName"].toString();
            attr.m_bitrate = MusicUtils::Number::transfromBitrateToNormal(bitRate / 1000);
            info->m_songAttrs.append(attr);
        }
    }
}

void MusicKGQueryInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key, const QString &quality, bool all)
{
    if(all)
    {
        readFromMusicSongAttribute(info, key["hash"].toString());
        readFromMusicSongAttribute(info, key["128hash"].toString());
        readFromMusicSongAttribute(info, key["320hash"].toString());
        readFromMusicSongAttribute(info, key["sqhash"].toString());
    }
    else
    {
        if(quality == QObject::tr("SD"))
        {
            readFromMusicSongAttribute(info, key["hash"].toString());
            readFromMusicSongAttribute(info, key["128hash"].toString());
        }
        else if(quality == QObject::tr("SQ"))
        {
            readFromMusicSongAttribute(info, key["320hash"].toString());
        }
        else if(quality == QObject::tr("CD"))
        {
            readFromMusicSongAttribute(info, key["sqhash"].toString());
        }
    }
}

void MusicKGQueryInterface::readFromMusicSongLrcAndPicture(MusicObject::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty())
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_SONG_INFO_URL, false).arg(info->m_songId));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KG_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

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
    const QVariant &data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["errcode"].toInt() == 0 && value.contains("data"))
        {
            value = value["data"].toMap();
            info->m_artistId = QString::number(value["singerid"].toULongLong());
            info->m_smallPicUrl = value["imgurl"].toString().replace("{size}", "480");
            info->m_lrcUrl = MusicUtils::Algorithm::mdII(KG_SONG_LRC_URL, false)
                                                    .arg(value["songname"].toString()).arg(info->m_songId)
                                                    .arg(value["duration"].toInt() * 1000);
        }
    }
}

void MusicKGQueryInterface::readFromMusicSongAlbumInfo(MusicResultsItem *info, const QString &album)
{
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_ALBUM_INFO_URL, false).arg(album));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KG_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

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
    const QVariant &data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("data"))
        {
            value = value["data"].toMap();
            info->m_nickName = value["albumname"].toString();
            info->m_description = info->m_nickName + TTK_STR_SPLITER +
                                  value["language"].toString() + TTK_STR_SPLITER +
                                  value["company"].toString() + TTK_STR_SPLITER +
                                  value["publishtime"].toString().left(10);
//            info->m_coverUrl = value["imgurl"].toString();
        }
    }
}
