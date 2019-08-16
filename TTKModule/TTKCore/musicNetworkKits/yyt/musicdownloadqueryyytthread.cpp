#include "musicdownloadqueryyytthread.h"
#include "musicsemaphoreloop.h"
#include "musicnumberutils.h"
#include "musiccoreutils.h"
#include "musictime.h"
#include "musicotherdefine.h"
#///QJson import
#include "qjson/parser.h"

const QString BD_MV_INFO_URL       = "dVBTZDgydDQvak9sYkp6c3VhYTZqV3k2NG9RK1dxcjNPYlZLWVZUWGx0aXlSRmY1YlBkV0ZGa0F5L2Y1U285M3VNTWtSYUI1N0tCRUNuK29VNUtqTXN6V0Y1TDZkYWJFS0Nqa280MjZHTnc4M1l3cUhqVldEOU9iblpSU0xvMzhSY0sxM1ZNRTZaUzM5RnhWRUJuWjRtdEJwWmFoNy9PYw==";
const QString BD_MV_INFO_ATTR_URL  = "S2p0SWNvNVNIVUEzV1QxOE83eTRsM0EwR3dGMUV2SXVNNWs1bW1iNUN1L0lxNHFCbXNCMDlkVjhZdFdzZGlvNVhJZzZOLzBUZE1MYjRnbGxkRzhiN2c9PQ==";
const QString BD_MV_INFO_ID        = "Q3o1b2RqcE1Tam5Vc2FnSQ==";
const QString BD_MV_INFO_DID       = "R2hwbW1ZRVI0cjV2Qm1lT0RrQjBrbHVFN3hLUnJyWDh3VDYzeU5VemxoQUZ0bGtC";
const QString BD_MV_INFO_DV        = "cEoxTGtxZW9qQ1d0UHhZZWpabnNnSlBLTWV5Z055RThINHA2V3ZjNWs4V0RPUXVLekdoVE56Qzd2YW1YbEwwbQ==";

MusicDownLoadQueryYYTThread::MusicDownLoadQueryYYTThread(QObject *parent)
    : MusicDownLoadQueryMovieThread(parent)
{
    m_queryServer = QUERY_YYT_INTERFACE;
}

void MusicDownLoadQueryYYTThread::startToSearch(QueryType type, const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(BD_MV_INFO_URL, false).arg(text);
    m_searchText = text.trimmed();
    m_currentType = type;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("App-Id", MusicUtils::Algorithm::mdII(BD_MV_INFO_ID, false).toUtf8());
    request.setRawHeader("Device-Id", MusicUtils::Algorithm::mdII(BD_MV_INFO_DID, false).toUtf8());
    request.setRawHeader("Device-V", MusicUtils::Algorithm::mdII(BD_MV_INFO_DV, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryYYTThread::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    emit clearAllItems();
    m_musicSongInfos.clear();

    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();///Get all the data obtained by request
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("code") && value["code"].toInt() == 0 &&
               value.contains("msg") && value["msg"].toString() == "SUCCESS")
            {
                const QVariantList &datas = value["data"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();

                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_songId = QString::number(value["videoId"].toULongLong());
                    musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["title"].toString());

                    const QVariantList &artistsList = value["artists"].toList();
                    foreach(const QVariant &var, artistsList)
                    {
                        if(var.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artMap = var.toMap();
                        musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(artMap["artistName"].toString());
                    }

                    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
                    readFromMusicMVAttribute(&musicInfo);
                    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }

                    musicInfo.m_timeLength = value["lasted"].toString();
                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_time = musicInfo.m_timeLength;
                    item.m_type = mapQueryServerString();
                    emit createSearchedItem(item);

                    musicInfo.m_songId = MUSIC_YYT_PREFIX + musicInfo.m_songId;
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryYYTThread::readFromMusicMVAttribute(MusicObject::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty() || !m_manager)
    {
        return;
    }

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(BD_MV_INFO_ATTR_URL, false).arg(info->m_songId);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("App-Id", MusicUtils::Algorithm::mdII(BD_MV_INFO_ID, false).toUtf8());
    request.setRawHeader("Device-Id", MusicUtils::Algorithm::mdII(BD_MV_INFO_DID, false).toUtf8());
    request.setRawHeader("Device-V", MusicUtils::Algorithm::mdII(BD_MV_INFO_DV, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get(request);
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
        if(value.contains("code") && value["code"].toInt() == 0 &&
           value.contains("msg") && value["msg"].toString() == "SUCCESS")
        {
            value = value["data"].toMap();
            const QString &duration = MusicTime::msecTime2LabelJustified(value["duration"].toInt()*1000);
            if(value.contains("hdUrl"))
            {
                readFromMusicMVAttribute(info, value["hdUrl"].toString(), MusicUtils::Number::size2Label(value["hdVideoSize"].toInt()), duration);
            }
            if(value.contains("uhdUrl"))
            {
                readFromMusicMVAttribute(info, value["uhdUrl"].toString(), MusicUtils::Number::size2Label(value["uhdVideoSize"].toInt()), duration);
            }
            if(value.contains("shdUrl"))
            {
                readFromMusicMVAttribute(info, value["shdUrl"].toString(), MusicUtils::Number::size2Label(value["shdVideoSize"].toInt()), duration);
            }
        }
    }
}

void MusicDownLoadQueryYYTThread::readFromMusicMVAttribute(MusicObject::MusicSongInformation *info, const QString &url, const QString &size, const QString &duration)
{
    if(url.isEmpty())
    {
        return;
    }

    QStringList datas = url.split("?");
    if(datas.count() == 2)
    {
        QString v = datas.front();
        MusicObject::MusicSongAttribute attr;
        attr.m_url = url;
        attr.m_size = size;
        attr.m_format = MusicUtils::String::StringSplite(v);
        attr.m_duration = duration;
        v = datas.back();
        foreach(QString var, v.split("&"))
        {
            if(var.contains("br="))
            {
                int bitRate = var.remove("br=").toInt();
                if(bitRate <= 500)
                    attr.m_bitrate = MB_250;
                else if(bitRate > 500 && bitRate <= 1000)
                    attr.m_bitrate = MB_500;
                else if(bitRate > 1000 && bitRate <= 2000)
                    attr.m_bitrate = MB_750;
                else if(bitRate > 2000)
                    attr.m_bitrate = MB_1000;
            }
        }
        info->m_songAttrs.append(attr);
    }
}
