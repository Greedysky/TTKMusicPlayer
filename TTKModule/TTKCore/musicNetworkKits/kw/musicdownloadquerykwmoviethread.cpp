#include "musicdownloadquerykwmoviethread.h"
#include "musicdownloadqueryyytthread.h"
#include "musicsemaphoreloop.h"
#include "musicnumberutils.h"
#include "musiccoreutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"
#include "qalg/qdeswrap.h"

MusicDownLoadQueryKWMovieThread::MusicDownLoadQueryKWMovieThread(QObject *parent)
    : MusicDownLoadQueryMovieThread(parent)
{
    m_queryServer = QUERY_KW_INTERFACE;
    m_pageSize = 40;
}

void MusicDownLoadQueryKWMovieThread::startToSearch(QueryType type, const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(KW_SONG_SEARCH_URL, false).arg(text).arg(0).arg(m_pageSize);
    m_searchText = text.trimmed();
    m_currentType = type;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryKWMovieThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(KW_AR_MV_URL, false).arg(m_searchText).arg(m_pageSize).arg(offset);
    m_pageTotal = 0;
    m_pageSize = 20;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(pageDownLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryKWMovieThread::startToSingleSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSingleSearch %2").arg(getClassName()).arg(text));

    m_searchText = text.trimmed();
    m_interrupt = true;

    QTimer::singleShot(MT_MS, this, SLOT(singleDownLoadFinished()));
}

void MusicDownLoadQueryKWMovieThread::downLoadFinished()
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
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes.replace("'", "\""), &ok);

        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("abslist"))
            {
                const QVariantList &datas = value["abslist"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = MusicUtils::String::illegalCharactersReplaced(value["ARTIST"].toString());
                    musicInfo.m_songName = MusicUtils::String::illegalCharactersReplaced(value["SONGNAME"].toString());
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["DURATION"].toInt()*1000);

                    musicInfo.m_songId = value["MUSICRID"].toString().remove("MUSIC_");
                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
                    readFromMusicMVAttribute(&musicInfo, value["FORMATS"].toString());
                    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                      continue;
                    }

                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_time = musicInfo.m_timeLength;
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

void MusicDownLoadQueryKWMovieThread::pageDownLoadFinished()
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
        const QVariant &data = parser.parse(bytes.replace("'", "\""), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            m_pageTotal = value["total"].toString().toLongLong();
            if(value.contains("mvlist"))
            {
                const QVariantList &datas = value["mvlist"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();

                    if(m_interrupt) return;

                    MusicResultsItem info;
                    info.m_id = value["musicid"].toString();
                    info.m_coverUrl = value["pic"].toString();
                    if(!info.m_coverUrl.contains("http://") && !info.m_coverUrl.contains(COVER_URL_NULL))
                    {
                        info.m_coverUrl = MusicUtils::Algorithm::mdII(KW_MV_COVER_URL, false) + info.m_coverUrl;
                    }
                    info.m_name = value["name"].toString();
                    info.m_updateTime.clear();
                    emit createMovieInfoItem(info);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 pageDownLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryKWMovieThread::singleDownLoadFinished()
{
    M_LOGGER_INFO(QString("%1 singleDownLoadFinished").arg(getClassName()));

    emit clearAllItems();
    m_musicSongInfos.clear();
    m_interrupt = false;

    MusicObject::MusicSongInformation musicInfo;
    musicInfo.m_songId = m_searchText;
    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
    readFromMusicMVInfo(&musicInfo);
    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;
    readFromMusicMVAttribute(&musicInfo, QString("MP4UL|MP4L|MP4HV|MP4"));
    if(m_interrupt || !m_manager || m_stateCode != MusicObject::NetworkInit) return;

    if(!musicInfo.m_songAttrs.isEmpty())
    {
        MusicSearchedItem item;
        item.m_songName = musicInfo.m_songName;
        item.m_singerName = musicInfo.m_singerName;
        item.m_time = musicInfo.m_timeLength;
        item.m_type = mapQueryServerString();
        emit createSearchedItem(item);
        m_musicSongInfos << musicInfo;
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 singleDownLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryKWMovieThread::readFromMusicMVAttribute(MusicObject::MusicSongInformation *info, const QString &format)
{
    if(info->m_songId.isEmpty() || !m_manager)
    {
        return;
    }

    foreach(const QString &v, format.split('|'))
    {
        if(v.contains("MP4L"))
        {
            readFromMusicMVAttribute(info, "MP4L", MB_250);
        }
        else if(v.contains("MP4HV"))
        {
            readFromMusicMVAttribute(info, "MP4HV", MB_750);
        }
        else if(v.contains("MP4UL"))
        {
            readFromMusicMVAttribute(info, "MP4UL", MB_1000);
        }
        else if(v.contains("MP4"))
        {
            readFromMusicMVAttribute(info, "MP4", MB_500);
        }
    }
}

void MusicDownLoadQueryKWMovieThread::readFromMusicMVAttribute(MusicObject::MusicSongInformation *info, const QString &format, int bitrate)
{
    if(info->m_songId.isEmpty() || !m_manager)
    {
        return;
    }

    QDesWrap des;
    const QByteArray &parameter = des.encrypt(MusicUtils::Algorithm::mdII(KW_MV_ATTR_URL, false).arg(info->m_songId).arg(format).toUtf8(),
                                              MusicUtils::Algorithm::mdII(_SIGN, ALG_LOW_KEY, false).toUtf8());
    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(KW_MV_URL, false).arg(QString(parameter));

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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

    const QByteArray &bytes = reply->readAll();
    if(!bytes.isEmpty() && !bytes.contains("res not found"))
    {
        const QString text(bytes);
        QRegExp regx(".*url=(.*)\r\nsig=");

        if(text.indexOf(regx) != -1)
        {
            MusicObject::MusicSongAttribute attr;
            attr.m_url = regx.cap(1);
            attr.m_bitrate = bitrate;
            attr.m_format = "mp4";
            if(attr.m_url.isEmpty() || info->m_songAttrs.contains(attr))
            {
                return;
            }

            if(!findUrlFileSize(&attr)) return;
            info->m_songAttrs.append(attr);
        }
    }
}

void MusicDownLoadQueryKWMovieThread::readFromMusicMVInfo(MusicObject::MusicSongInformation *info)
{
    if(info->m_songId.isEmpty() || !m_manager)
    {
        return;
    }

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(KW_MV_HOME_URL, false).arg(info->m_songId);
    info->m_songName = "Not Found";
    info->m_singerName = "Anonymous";

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
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

    const QString text(reply->readAll());
    QRegExp regx("<h1 title=\"([^<]+)\">[^>]+>([^<]+)</span></h1>");

    if(text.indexOf(regx) != -1)
    {
        info->m_songName = regx.cap(1);
        info->m_singerName = regx.cap(2);
    }
}
