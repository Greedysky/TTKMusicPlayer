#include "musicdownloadquerybdmoviethread.h"
#include "musicdownloadqueryyytthread.h"
#include "musicsemaphoreloop.h"
#include "musiccoreutils.h"
#include "musicnumberutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryBDMovieThread::MusicDownLoadQueryBDMovieThread(QObject *parent)
    : MusicDownLoadQueryMovieThread(parent)
{
    m_queryServer = "Baidu";
}

QString MusicDownLoadQueryBDMovieThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryBDMovieThread::startToSearch(QueryType type, const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    m_searchText = text.trimmed();
    m_currentType = type;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_SONG_SEARCH_URL, false).arg(text).arg(0).arg(50);
    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryBDMovieThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    m_pageTotal = 0;
    m_pageSize = 20;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_AR_MV_URL, false).arg(m_searchText).arg(offset*m_pageSize);
    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(pageDownLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryBDMovieThread::startToSingleSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSingleSearch %2").arg(getClassName()).arg(text));
    m_searchText = text.trimmed();
    deleteAll();
    m_interrupt = true;

    QTimer::singleShot(MT_MS, this, SLOT(singleDownLoadFinished()));
}

void MusicDownLoadQueryBDMovieThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["error_code"].toInt() == 22000 && value.contains("result"))
            {
                value = value["result"].toMap();
                value = value["song_info"].toMap();
                QVariantList datas = value["song_list"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = value["author"].toString();
                    musicInfo.m_songName = value["title"].toString();
                    musicInfo.m_timeLength = "-";

                    if(value["has_mv"].toInt() == 1)
                    {
                        musicInfo.m_songId = value["song_id"].toString();
                        if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        readFromMusicMVAttribute(&musicInfo, false);
                        if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    }

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }

                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_time = findTimeStringByAttrs(musicInfo.m_songAttrs);
                    item.m_type = mapQueryServerString();
                    emit createSearchedItems(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    ///extra yyt movie
    if(m_queryExtraMovie && m_currentType == MovieQuery)
    {
        MusicSemaphoreLoop loop;
        MusicDownLoadQueryYYTThread *yyt = new MusicDownLoadQueryYYTThread(this);
        connect(yyt, SIGNAL(createSearchedItems(MusicSearchedItem)), SIGNAL(createSearchedItems(MusicSearchedItem)));
        connect(yyt, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
        yyt->startToSearch(MusicDownLoadQueryYYTThread::MovieQuery, m_searchText);
        loop.exec();
        m_musicSongInfos << yyt->getMusicSongInfos();
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryBDMovieThread::pageDownLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 pageDownLoadFinished").arg(getClassName()));
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data") && value["errorCode"].toInt() == 22000)
            {
                readFromMusicMVCount();

                value = value["data"].toMap();
                QString html = value["html"].toString();
                QRegExp regx("data-mv=\\\"([^\"]+).*title=\\\"([^\"]+).*org_src=\\\"([^\"]+).*>");
                regx.setMinimal(true);
                int pos = html.indexOf(regx);
                while(pos != -1)
                {
                    if(m_interrupt) return;

                    MusicPlaylistItem info;
                    info.m_id = regx.cap(1).remove("/playmv/");
                    info.m_coverUrl = regx.cap(3).remove("@s_0,w_160,h_90");
                    info.m_name = regx.cap(2).remove("MV");
                    info.m_updateTime.clear();
                    emit createMovieInfoItem(info);

                    pos += regx.matchedLength();
                    pos = regx.indexIn(html, pos);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 pageDownLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryBDMovieThread::singleDownLoadFinished()
{
    M_LOGGER_INFO(QString("%1 singleDownLoadFinished").arg(getClassName()));

    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo
    m_interrupt = false;

    MusicObject::MusicSongInformation musicInfo;
    musicInfo.m_songId = m_searchText;
    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    readFromMusicMVAttribute(&musicInfo, true);
    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

    if(!musicInfo.m_songAttrs.isEmpty())
    {
        MusicSearchedItem item;
        item.m_songName = musicInfo.m_songName;
        item.m_singerName = musicInfo.m_singerName;
        item.m_time = findTimeStringByAttrs(musicInfo.m_songAttrs);
        item.m_type = mapQueryServerString();
        emit createSearchedItems(item);
        m_musicSongInfos << musicInfo;
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 singleDownLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryBDMovieThread::readFromMusicMVCount()
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 readFromMusicMVCount").arg(getClassName()));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_AR_MV_PG_URL, false).arg(m_searchText);
    m_pageTotal = MU_MAX;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QString html(reply->readAll());
    QRegExp regx("<a class=\"list\" hidefocus=\"true\" href=\"#\">MV(.*)</a>");
    regx.setMinimal(true);
    if(html.indexOf(regx) != -1)
    {
        m_pageTotal = regx.cap(1).remove('(').remove(')').toInt();
    }
}

void MusicDownLoadQueryBDMovieThread::readFromMusicMVAttribute(MusicObject::MusicSongInformation *info, bool more)
{
    if(info->m_songId.isEmpty() || !m_manager)
    {
        return;
    }

    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_MV_INFO_URL, false).arg(info->m_songId);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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
        if(value["error_code"].toInt() == 22000 && value.contains("result"))
        {
            value = value["result"].toMap();

            if(more)
            {
                QVariantMap mvInfoMap = value["mv_info"].toMap();
                info->m_singerName = mvInfoMap["artist"].toString();
                info->m_songName = mvInfoMap["title"].toString();
                info->m_timeLength = "-";
            }

            value = value["files"].toMap();
            value = value[value.keys().first()].toMap();
            QString path = value["file_link"].toString();
            if(path.contains("video-url"))
            {
                path = path.split("/").back();
            }
            else
            {
                path = path.split("/").back().split("_").front();
            }

            if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
            readFromMusicMVAttribute(info, path);
            if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
        }
    }
}

void MusicDownLoadQueryBDMovieThread::readFromMusicMVAttribute(MusicObject::MusicSongInformation *info, const QString &id)
{
    if(id.isEmpty() || !m_manager)
    {
        return;
    }

    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_MV_INFO_ATTR_URL, false).arg(id);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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
        if(value.contains("error") && value["error"].toString() == "false")
        {
            QString duration = MusicTime::msecTime2LabelJustified(value["duration"].toInt()*1000);
            if(value.contains("hdVideoUrl"))
            {
                readFromMusicMVAttribute(info, value["hdVideoUrl"].toString(), duration);
            }
            if(value.contains("hcVideoUrl"))
            {
                readFromMusicMVAttribute(info, value["hcVideoUrl"].toString(), duration);
            }
            if(value.contains("heVideoUrl"))
            {
                readFromMusicMVAttribute(info, value["heVideoUrl"].toString(), duration);
            }
        }
    }
}

void MusicDownLoadQueryBDMovieThread::readFromMusicMVAttribute(MusicObject::MusicSongInformation *info,
                                                               const QString &url, const QString &duration)
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
        if(!findUrlFileSize(&attr)) return;
        attr.m_format = MusicUtils::Core::fileSuffix(v);
        attr.m_duration = duration;
        v = datas.back();
        foreach(QString var, v.split("&"))
        {
            if(var.contains("br="))
            {
                int bitRate = var.remove("br=").toInt();
                if(bitRate <= 375)
                    attr.m_bitrate = MB_250;
                else if(bitRate > 375 && bitRate <= 625)
                    attr.m_bitrate = MB_500;
                else if(bitRate > 625 && bitRate <= 875)
                    attr.m_bitrate = MB_750;
                else if(bitRate > 875)
                    attr.m_bitrate = MB_1000;
            }
        }
        info->m_songAttrs.append(attr);
    }
}
