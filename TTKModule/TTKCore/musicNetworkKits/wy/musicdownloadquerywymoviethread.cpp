#include "musicdownloadquerywymoviethread.h"
#include "musicdownloadqueryyytthread.h"
#include "musicsemaphoreloop.h"
#include "musiccoreutils.h"
#include "musicnumberutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryWYMovieThread::MusicDownLoadQueryWYMovieThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "WangYi";
}

QString MusicDownLoadQueryWYMovieThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryWYMovieThread::startToSearch(QueryType type, const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    m_searchText = text.trimmed();
    m_currentType = type;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WY_SONG_SEARCH_URL, false);
    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    makeTokenQueryQequest(&request);
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->post(request, MusicUtils::Algorithm::mdII(WY_SONG_SQUERY_URL, false).arg(text).arg(0).toUtf8());
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryWYMovieThread::startToSingleSearch(const QString &text)
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

void MusicDownLoadQueryWYMovieThread::downLoadFinished()
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
        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("code") && value["code"].toInt() == 200)
            {
                value = value["result"].toMap();
                QVariantList datas = value["songs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    int mvid = value["mvid"].toLongLong();
                    if(mvid == 0)
                    {
                        continue;
                    }

                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    startMVListQuery(mvid);
                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
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

void MusicDownLoadQueryWYMovieThread::singleDownLoadFinished()
{
    M_LOGGER_INFO(QString("%1 singleDownLoadFinished").arg(getClassName()));

    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo
    m_interrupt = false;

    int mvid = m_searchText.toLongLong();
    if(mvid != 0)
    {
        if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
        startMVListQuery(mvid);
        if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 singleDownLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryWYMovieThread::startMVListQuery(int id)
{
    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    QByteArray parameter = makeTokenQueryUrl(&request,
               MusicUtils::Algorithm::mdII(WY_SONG_MV_N_URL, false),
               MusicUtils::Algorithm::mdII(WY_SONG_MV_NDT_URL, false).arg(id));
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->post(request, parameter);
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
        if(value.contains("code") && value["code"].toInt() == 200)
        {
            value = value["data"].toMap();
            MusicObject::MusicSongInformation musicInfo;
            musicInfo.m_songName = value["name"].toString();
            musicInfo.m_singerName = value["artistName"].toString();
            musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt());

            value = value["brs"].toMap();
            foreach(const QString &key, value.keys())
            {
                int bitRate = key.toInt();
                MusicObject::MusicSongAttribute attr;
                if(bitRate <= 375)
                    attr.m_bitrate = MB_250;
                else if(bitRate > 375 && bitRate <= 625)
                    attr.m_bitrate = MB_500;
                else if(bitRate > 625 && bitRate <= 875)
                    attr.m_bitrate = MB_750;
                else if(bitRate > 875)
                    attr.m_bitrate = MB_1000;

                attr.m_url = value[key].toString();
                attr.m_format = MusicUtils::Core::fileSuffix(attr.m_url);
                if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                attr.m_size = MusicUtils::Number::size2Label(getUrlFileSize(attr.m_url));
                if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                musicInfo.m_songAttrs.append(attr);
            }

            if(musicInfo.m_songAttrs.isEmpty())
            {
                return;
            }

            MusicSearchedItem item;
            item.m_songName = musicInfo.m_songName;
            item.m_singerName = musicInfo.m_singerName;
            item.m_time = musicInfo.m_timeLength;
            item.m_type = mapQueryServerString();
            emit createSearchedItems(item);
            m_musicSongInfos << musicInfo;
        }
    }
}
