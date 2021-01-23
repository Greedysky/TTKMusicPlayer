#include "musicmgqueryinterface.h"
#include "musicsemaphoreloop.h"
#include "musicabstractqueryrequest.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

void MusicMGQueryInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const bool status, int bitrate, const QString &suffix)
{
    if(info->m_songId.isEmpty() || !status)
    {
        return;
    }

    QString qulity = "PQ";
    switch(bitrate)
    {
        case MB_128: qulity = "PQ"; break;
        case MB_192: qulity = "Z3D"; break;
        case MB_320: qulity = "HQ"; break;
        case MB_750: qulity = "SQ"; break;
        case MB_1000: qulity = "ZQ"; break;
        default: break;
    }

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(MG_SONG_PATH_URL, false).arg(info->m_songId).arg(qulity));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(MG_UA_URL, ALG_UA_KEY, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(MG_REFERER_URL, false).toUtf8());
    request.setRawHeader("uid", "1234");
    request.setRawHeader("channel", "mx");
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
        if(value.contains("code") && value["code"].toString() == "000000")
        {
            value = value["data"].toMap();
            MusicObject::MusicSongAttribute attr;
            attr.m_url = value["url"].toString();
            if(attr.m_url.isEmpty())
            {
                return;
            }

            attr.m_size = STRING_NULL;
            attr.m_format = suffix;
            attr.m_bitrate = bitrate;
            info->m_songAttrs.append(attr);

            //extend
            value = value["songItem"].toMap();
            if(info->m_albumId.isEmpty())
            {
                info->m_albumId = value["albumId"].toString();
            }

            if(info->m_albumName.isEmpty())
            {
                info->m_albumName = value["album"].toString();
            }

            if(info->m_lrcUrl.isEmpty())
            {
                info->m_lrcUrl = MusicUtils::Algorithm::mdII(MG_SONG_LRC_URL, false).arg(value["copyrightId"].toString());
            }

            if(info->m_smallPicUrl.isEmpty())
            {
                const QVariantList &datas = value["albumImgs"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    info->m_smallPicUrl = value["img"].toString();
                    break;
                }
            }
        }
    }
}

void MusicMGQueryInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QString &key, const QString &quality, bool all)
{
    if(key.isEmpty())
    {
        return;
    }

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(MG_SONG_INFO_URL, false).arg(key));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(MG_UA_URL, ALG_UA_KEY, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(MG_REFERER_URL, false).toUtf8());
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
            readFromMusicSongAttribute(info, value, quality, all);
        }
    }
}

void MusicMGQueryInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key, const QString &quality, bool all)
{
    if(all)
    {
        readFromMusicSongAttribute(info, true, MB_128, MP3_FILE_PREFIX); //PQ
        readFromMusicSongAttribute(info, key["has3Dqq"].toInt() == 1, MB_192, MP3_FILE_PREFIX); //3D
        readFromMusicSongAttribute(info, key["hasHQqq"].toInt() == 1, MB_320, MP3_FILE_PREFIX); //HQ
        readFromMusicSongAttribute(info, key["hasSQqq"].toInt() == 1, MB_750, FLC_FILE_PREFIX); //SQ
        readFromMusicSongAttribute(info, key["has24Bitqq"].toInt() == 1, MB_1000, FLC_FILE_PREFIX); //ZQ
    }
    else
    {
        if(quality == QObject::tr("SD"))
        {
            readFromMusicSongAttribute(info, true, MB_128, MP3_FILE_PREFIX); //PQ
        }
        else if(quality == QObject::tr("HQ"))
        {
            readFromMusicSongAttribute(info, key["has3Dqq"].toInt() == 1, MB_192, MP3_FILE_PREFIX); //3D
        }
        else if(quality == QObject::tr("SQ"))
        {
            readFromMusicSongAttribute(info, key["hasHQqq"].toInt() == 1, MB_320, MP3_FILE_PREFIX); //HQ
        }
        else if(quality == QObject::tr("CD"))
        {
            readFromMusicSongAttribute(info, key["hasSQqq"].toInt() == 1, MB_750, FLC_FILE_PREFIX); //SQ
            readFromMusicSongAttribute(info, key["has24Bitqq"].toInt() == 1, MB_1000, FLC_FILE_PREFIX); //ZQ
        }
    }
}

void MusicMGQueryInterface::readFromMusicSongAttributeNew(MusicObject::MusicSongInformation *info, const QVariantMap &key, const QString &quality, bool all)
{
    if(all)
    {
        readFromMusicSongAttribute(info, true, MB_128, MP3_FILE_PREFIX); //PQ
        readFromMusicSongAttribute(info, !key["d3"].isNull(), MB_192, MP3_FILE_PREFIX); //3D
        readFromMusicSongAttribute(info, !key["hq"].isNull(), MB_320, MP3_FILE_PREFIX); //HQ
        readFromMusicSongAttribute(info, !key["sq"].isNull(), MB_750, FLC_FILE_PREFIX); //SQ
        readFromMusicSongAttribute(info, !key["bit24"].isNull(), MB_1000, FLC_FILE_PREFIX); //ZQ
    }
    else
    {
        if(quality == QObject::tr("SD"))
        {
            readFromMusicSongAttribute(info, true, MB_128, MP3_FILE_PREFIX); //PQ
        }
        else if(quality == QObject::tr("HQ"))
        {
            readFromMusicSongAttribute(info, !key["d3"].isNull(), MB_192, MP3_FILE_PREFIX); //3D
        }
        else if(quality == QObject::tr("SQ"))
        {
            readFromMusicSongAttribute(info, !key["hq"].isNull(), MB_320, MP3_FILE_PREFIX); //HQ
        }
        else if(quality == QObject::tr("CD"))
        {
            readFromMusicSongAttribute(info, !key["sq"].isNull(), MB_750, FLC_FILE_PREFIX); //SQ
            readFromMusicSongAttribute(info, !key["bit24"].isNull(), MB_1000, FLC_FILE_PREFIX); //ZQ
        }
    }
}

void MusicMGQueryInterface::setTimeLength(MusicObject::MusicSongInformation *info)
{
    if(info->m_timeLength != STRING_NULL)
    {
        return;
    }

    for(const MusicObject::MusicSongAttribute &attr : info->m_songAttrs)
    {
        if(attr.m_bitrate != MB_1000)
        {
            const qint64 size = MusicAbstractQueryRequest::getUrlFileSize(attr.m_url);
            if(size < 0)
            {
                continue;
            }

            info->m_timeLength = MusicTime::msecTime2LabelJustified(size * 8 / attr.m_bitrate);
            break;
        }
    }
}
