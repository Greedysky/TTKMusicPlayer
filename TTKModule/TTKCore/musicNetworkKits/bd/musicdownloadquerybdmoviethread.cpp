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
    m_queryServer = QUERY_BD_INTERFACE;
    m_pageSize = 30;
}

void MusicDownLoadQueryBDMovieThread::startToSearch(QueryType type, const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(BD_SONG_SEARCH_URL, false).arg(text).arg(0).arg(m_pageSize);
    m_searchText = text.trimmed();
    m_currentType = type;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

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
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(BD_AR_MV_URL, false).arg(m_searchText).arg(offset*m_pageSize);
    m_pageTotal = 0;
    m_pageSize = 20;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

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
    deleteAll();

    m_searchText = text.trimmed();
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
    emit clearAllItems();
    m_musicSongInfos.clear();
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["error_code"].toInt() == 22000 && value.contains("result"))
            {
                value = value["result"].toMap();
                value = value["song_info"].toMap();
                const QVariantList &datas = value["song_list"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(value["author"].toString());
                    musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["title"].toString());
                    musicInfo.m_timeLength = "-";

                    if(value["has_mv"].toInt() == 1)
                    {
                        musicInfo.m_songId = value["song_id"].toString();
                        if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                        readFromMusicMVAttribute(&musicInfo, false);
                        if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
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
                    emit createSearchedItem(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    ///extra yyt movie
    if(m_queryExtraMovie && m_currentType == MovieQuery)
    {
        MusicSemaphoreLoop loop;
        MusicDownLoadQueryYYTThread *d = new MusicDownLoadQueryYYTThread(this);
        connect(d, SIGNAL(createSearchedItem(MusicSearchedItem)), SIGNAL(createSearchedItem(MusicSearchedItem)));
        connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
        d->startToSearch(MusicDownLoadQueryYYTThread::MovieQuery, m_searchText);
        loop.exec();
        m_musicSongInfos << d->getMusicSongInfos();
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
        const QByteArray &bytes = m_reply->readAll();///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data") && value["errorCode"].toInt() == 22000)
            {
                m_pageTotal = DEFAULT_LEVEL_HIGHER;
                value = value["data"].toMap();

                const QString &html = value["html"].toString();
                QRegExp regx("<img src=\\\"([^\"]+).*>.*/playmv\\/([^\"]+)\\\".*title=\\\"([^\"]+).*a-tj='");
                regx.setMinimal(true);

                int pos = html.indexOf(regx);
                while(pos != -1)
                {
                    if(m_interrupt) return;

                    MusicResultsItem info;
                    info.m_id = regx.cap(2);
                    info.m_coverUrl = regx.cap(1).remove("@s_0,w_160,h_90");
                    info.m_name = regx.cap(3);
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

    emit clearAllItems();
    m_musicSongInfos.clear();
    m_interrupt = false;

    MusicObject::MusicSongInformation musicInfo;
    musicInfo.m_songId = m_searchText;
    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
    readFromMusicMVAttribute(&musicInfo, true);
    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;

    /// found playmv url request
    if(musicInfo.m_songAttrs.isEmpty())
    {
        readFromMusicMVAttributeWeb(&musicInfo);
    }

    if(!musicInfo.m_songAttrs.isEmpty())
    {
        MusicSearchedItem item;
        item.m_songName = musicInfo.m_songName;
        item.m_singerName = musicInfo.m_singerName;
        item.m_time = findTimeStringByAttrs(musicInfo.m_songAttrs);
        item.m_type = mapQueryServerString();
        emit createSearchedItem(item);
        m_musicSongInfos << musicInfo;
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 singleDownLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryBDMovieThread::readFromMusicMVAttributeWeb(MusicObject::MusicSongInformation *info)
{
    if(!m_manager)
    {
        return;
    }

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(BD_MV_INFO_ATTR_W_URL, false).arg(info->m_songId);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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

    const QString html(reply->readAll());
    QRegExp regx("data.push(.*);");
    regx.setMinimal(true);

    if(html.indexOf(regx) != -1)
    {
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(regx.cap(1).remove('(').remove(')').toUtf8(), &ok);
        if(ok)
        {
            const QVariantMap &value = data.toMap();
            const QString &type = value["source"].toString();

            if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
            if(type == QUERY_YYT_INTERFACE)
            {
                const QString &vars = QString("videoId=%1").arg(value["tvid"].toString());
                readFromMusicMVAttributeYYT(info, vars, true);
            }
            else if(type == "iqiyi")
            {
                const QString &vars = QString("tvid=%1&vid=%2").arg(value["tvid"].toString()).arg(value["vid"].toString());
                readFromMusicMVAttributeIQY(info, vars, true);
            }
            else if(type == "own")
            {
                info->m_songName = value["title"].toString();
                info->m_singerName = value["author"].toString();
                info->m_timeLength = MusicTime::msecTime2LabelJustified(value["time"].toInt()*1000);

                MusicObject::MusicSongAttribute attr;
                attr.m_url = value["file_link"].toString();
                attr.m_duration = info->m_timeLength;
                attr.m_bitrate = MB_500;
                attr.m_format = MusicUtils::String::StringSplite(attr.m_url);
                if(!findUrlFileSize(&attr)) return;
                info->m_songAttrs.append(attr);
            }
            if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
        }
    }
}

void MusicDownLoadQueryBDMovieThread::readFromMusicMVAttribute(MusicObject::MusicSongInformation *info, bool more)
{
    if(info->m_songId.isEmpty() || !m_manager)
    {
        return;
    }

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(BD_MV_INFO_URL, false).arg(info->m_songId);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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
        const QVariantMap &value = data.toMap();
        if(value["re"].toInt() == 1)
        {
            const QString &type = value["source"].toString();
            const QString &vars = value["flashvars"].toString();

            if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
            if(type == QUERY_YYT_INTERFACE)
            {
                readFromMusicMVAttributeYYT(info, vars, more);
            }
            else if(type == "iqiyi")
            {
                readFromMusicMVAttributeIQY(info, vars, more);
            }
            if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
        }
    }
}

void MusicDownLoadQueryBDMovieThread::readFromMusicMVAttributeYYT(MusicObject::MusicSongInformation *info, const QString &var, bool more)
{
    if(!m_manager)
    {
        return;
    }

    QString videoId;
    foreach(QString ty, var.split("&"))
    {
        if(ty.contains("videoId="))
        {
            videoId = ty.remove("videoId=");
            break;
        }
    }

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(BD_MV_INFO_ATTR_Y_URL, false).arg(videoId);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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
        if(value.contains("error") && value["error"].toString() == "false")
        {
            value = value["videoInfo"].toMap();
            value = value["coreVideoInfo"].toMap();

            const QString &duration = MusicTime::msecTime2LabelJustified(value["duration"].toInt()*1000);

            if(more)
            {
                info->m_songName = value["videoName"].toString();
                info->m_singerName = value["artistNames"].toString();
                info->m_timeLength = duration;
            }

            const QVariantList &datas = value["videoUrlModels"].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                const int type = value["bitrateType"].toInt();
                if(type == 1)
                {
                    readFromMusicMVAttributeYYT(info, value, MB_500, duration);
                }
                else if(type == 2)
                {
                    readFromMusicMVAttributeYYT(info, value, MB_750, duration);
                }
                else if(type == 3)
                {
                    readFromMusicMVAttributeYYT(info, value, MB_1000, duration);
                }
            }
        }
    }
}

void MusicDownLoadQueryBDMovieThread::readFromMusicMVAttributeIQY(MusicObject::MusicSongInformation *info, const QString &var, bool more)
{
    if(info->m_songId.isEmpty() || !m_manager)
    {
        return;
    }

    QString vid, tvid;
    foreach(QString ty, var.split("&"))
    {
        if(ty.contains("vid="))
        {
            vid = ty.remove("vid=");
        }
        else if(ty.contains("tvid="))
        {
            tvid = ty.remove("tvid=");
        }
    }

    const QString &time = QString::number(MusicTime::timeStamp());
    const QString &host = MusicUtils::Algorithm::mdII(BD_MV_INFO_ATTR_I_URL, false);
    const QString &src = MusicUtils::Algorithm::mdII(BD_MV_INFO_ATTR_ID_URL, false).arg(tvid).arg(vid).arg(tvid).arg(time);

    int v8 = 0, v4 = 0;
    QString sufix;
    for(int i=0; i<8; ++i)
    {
        for(int j=0; j<4; ++j)
        {
            v4 = 13 * (66 * j + 27 * i) % 35;
            if(v4 >= 10)
            {
                v8 = v4 + 88;
            }
            else
            {
                v8 = v4 + 49;
            }
            sufix += char(v8);
        }
    }

    const QUrl &musicUrl = host + src + "&vf=" + MusicUtils::Algorithm::md5((src + sufix).toUtf8()).toHex();

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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
        if(value.contains("data") && value["code"].toString() == "A00000")
        {
            value = value["data"].toMap();
            value = value["vp"].toMap();

            const QString &prefix = value["du"].toString();
            QVariantList datas = value["tkl"].toList();
            if(datas.isEmpty())
            {
                return;
            }

            datas = datas.first().toMap()["vs"].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                const QVariantList &fsDatas = value["fs"].toList();
                if(fsDatas.isEmpty())
                {
                    continue;
                }

                MusicObject::MusicSongAttribute attr;
                switch(value["bid"].toInt())
                {
                    case 96: attr.m_bitrate = MB_250; break;
                    case 1: attr.m_bitrate = MB_500; break;
                    case 14:
                    case 21:
                    case 2: attr.m_bitrate = MB_750; break;
                    case 4:
                    case 17:
                    case 5:
                    case 18: attr.m_bitrate = MB_750; break;
                    default: attr.m_bitrate = MB_250; break;
                }

                value = fsDatas.first().toMap();
                attr.m_url = prefix + value["l"].toString();
                if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                readFromMusicMVAttributeIQY(&attr);
                if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                attr.m_duration = MusicTime::msecTime2LabelJustified(value["d"].toLongLong());
                attr.m_format = "flv";
                attr.m_size = MusicUtils::Number::size2Label(value["b"].toLongLong());
                info->m_songAttrs.append(attr);
            }

            if(more)
            {
                info->m_songName = "Default";
                info->m_singerName = "Default";
                info->m_timeLength = findTimeStringByAttrs(info->m_songAttrs);
            }
        }
    }
}

void MusicDownLoadQueryBDMovieThread::readFromMusicMVAttributeYYT(MusicObject::MusicSongInformation *info, const QVariantMap &key, int bitrate, const QString &duration)
{
    if(key.isEmpty())
    {
        return;
    }

    MusicObject::MusicSongAttribute attr;
    attr.m_url = key["videoUrl"].toString();
    attr.m_duration = duration;
    attr.m_bitrate = bitrate;

    const QStringList &datas = attr.m_url.split("?");
    if(datas.count() == 2)
    {
        const QString &v = datas.front();
        attr.m_format = MusicUtils::String::StringSplite(v);
        attr.m_size = MusicUtils::Number::size2Label(key["fileSize"].toLongLong());
        info->m_songAttrs.append(attr);
    }
}

void MusicDownLoadQueryBDMovieThread::readFromMusicMVAttributeIQY(MusicObject::MusicSongAttribute *attr)
{
    if(attr->m_url.isEmpty())
    {
        return;
    }

    const QUrl &musicUrl = attr->m_url;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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
        const QVariantMap &value = data.toMap();
        if(value.contains("l"))
        {
            attr->m_url = value["l"].toString();
        }
    }
}
