#include "musicdownloadqueryyytthread.h"
#include "musicsemaphoreloop.h"
#include "musicnumberutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

const QString BD_MV_INFO_URL       = "dVBTZDgydDQvak9sYkp6c3VhYTZqV3k2NG9RK1dxcjNPYlZLWVZUWGx0aXlSRmY1YlBkV0ZGa0F5L2Y1U285M3VNTWtSYUI1N0tCRUNuK29VNUtqTXN6V0Y1TDZkYWJFS0Nqa280MjZHTnc4M1l3cUhqVldEOU9iblpSU0xvMzhSY0sxM1ZNRTZaUzM5RnhWRUJuWjRtdEJwWmFoNy9PYw==";
const QString BD_MV_INFO_ATTR_URL  = "S2p0SWNvNVNIVUEzV1QxOE83eTRsM0EwR3dGMUV2SXVNNWs1bW1iNUN1L0lxNHFCbXNCMDlkVjhZdFdzZGlvNVhJZzZOLzBUZE1MYjRnbGxkRzhiN2c9PQ==";

MusicDownLoadQueryYYTThread::MusicDownLoadQueryYYTThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "YinYueTai";
}

QString MusicDownLoadQueryYYTThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryYYTThread::startToSearch(QueryType type, const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    m_searchText = text.trimmed();
    m_currentType = type;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_MV_INFO_URL, false).arg(text);
    deleteAll();

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("App-Id", "10201041");
    request.setRawHeader("Device-Id", "178bc560c9e8d719e048c7e8f2d25fcb");
    request.setRawHeader("Device-V", "QW5kcm9pZF80LjQuMl83NjgqMTE4NF8xMDAwMDEwMDA=");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryYYTThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request
        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("code") && value["code"].toInt() == 0 &&
               value.contains("msg") && value["msg"].toString() == "SUCCESS")
            {
                QVariantList datas = value["data"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();

                    MusicObject::MusicSongInfomation musicInfo;
                    musicInfo.m_songId = QString::number(value["videoId"].toULongLong());
                    musicInfo.m_songName = value["title"].toString();

                    QVariantList artistsList = value["artists"].toList();
                    foreach(const QVariant &var, artistsList)
                    {
                        if(var.isNull())
                        {
                            continue;
                        }

                        value = var.toMap();
                        musicInfo.m_singerName = value["artistName"].toString();
                    }

                    readFromMusicMVAttribute(&musicInfo, musicInfo.m_songId);
                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }
                    musicInfo.m_timeLength = musicInfo.m_songAttrs.first().m_duration;
                    MusicSearchedItem item;
                    item.m_songname = musicInfo.m_songName;
                    item.m_artistname = musicInfo.m_singerName;
                    item.m_time = musicInfo.m_timeLength;
                    item.m_type = mapQueryServerString();
                    emit createSearchedItems(item);

                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}

void MusicDownLoadQueryYYTThread::readFromMusicMVAttribute(MusicObject::MusicSongInfomation *info,
                                                           const QString &id)
{
    if(id.isEmpty() || !m_manager)
    {
        return;
    }

    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_MV_INFO_ATTR_URL, false).arg(id);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("App-Id", "10201041");
    request.setRawHeader("Device-Id", "178bc560c9e8d719e048c7e8f2d25fcb");
    request.setRawHeader("Device-V", "QW5kcm9pZF80LjQuMl83NjgqMTE4NF8xMDAwMDEwMDA=");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("code") && value["code"].toInt() == 0 &&
           value.contains("msg") && value["msg"].toString() == "SUCCESS")
        {
            value = value["data"].toMap();
            QString duration = MusicTime::msecTime2LabelJustified(value["duration"].toInt()*1000);
            if(value.contains("hdUrl"))
            {
                readFromMusicMVInfoAttribute(info, value["hdUrl"].toString(),
                                             MusicUtils::Number::size2Label(value["hdVideoSize"].toInt()), duration);
            }
            if(value.contains("uhdUrl"))
            {
                readFromMusicMVInfoAttribute(info, value["uhdUrl"].toString(),
                                             MusicUtils::Number::size2Label(value["uhdVideoSize"].toInt()), duration);
            }
            if(value.contains("shdUrl"))
            {
                readFromMusicMVInfoAttribute(info, value["shdUrl"].toString(),
                                             MusicUtils::Number::size2Label(value["shdVideoSize"].toInt()), duration);
            }
        }
    }
}

void MusicDownLoadQueryYYTThread::readFromMusicMVInfoAttribute(MusicObject::MusicSongInfomation *info,
                                                               const QString &url, const QString &size, const QString &duration)
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
        attr.m_format = v.right(v.length() - v.lastIndexOf(".") - 1);
        attr.m_duration = duration;
        v = datas.back();
        foreach(QString var, v.split("&"))
        {
            if(var.contains("br="))
            {
                int bitRate = var.remove("br=").toInt();
                if(bitRate > 0 && bitRate <= 1000)
                    attr.m_bitrate = MB_500;
                else if(bitRate > 1000 && bitRate <= 2000)
                    attr.m_bitrate = MB_750;
                else if(bitRate > 2000)
                    attr.m_bitrate = MB_1000;
                else
                    attr.m_bitrate = bitRate;
            }
        }
        info->m_songAttrs.append(attr);
    }
}
