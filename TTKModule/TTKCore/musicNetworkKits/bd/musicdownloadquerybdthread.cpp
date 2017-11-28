#include "musicdownloadquerybdthread.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryBDThread::MusicDownLoadQueryBDThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "Baidu";
}

QString MusicDownLoadQueryBDThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryBDThread::startToSearch(QueryType type, const QString &text)
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

void MusicDownLoadQueryBDThread::startToSingleSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSingleSearch %2").arg(getClassName()).arg(text));
    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    makeTokenQueryUrl(&request, text);

    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(singleDownLoadFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryBDThread::downLoadFinished()
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

                    musicInfo.m_songId = value["song_id"].toString();
                    musicInfo.m_artistId = value["ting_uid"].toString();
                    musicInfo.m_albumId = value["album_id"].toString();

                    if(!m_querySimplify)
                    {
                        musicInfo.m_lrcUrl = value["lrclink"].toString();
                        musicInfo.m_smallPicUrl = value["pic_small"].toString().replace(",w_90", ",w_500");
                        musicInfo.m_albumName = value["album_title"].toString();

                        if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        readFromMusicSongAttribute(&musicInfo, value["all_rate"].toString(), m_searchQuality, m_queryAllRecords);
                        if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                        if(musicInfo.m_songAttrs.isEmpty())
                        {
                            continue;
                        }

                        musicInfo.m_timeLength = findTimeStringByAttrs(musicInfo.m_songAttrs);
                        MusicSearchedItem item;
                        item.m_songName = musicInfo.m_songName;
                        item.m_singerName = musicInfo.m_singerName;
                        item.m_albumName = musicInfo.m_albumName;
                        item.m_time = musicInfo.m_timeLength;
                        item.m_type = mapQueryServerString();
                        emit createSearchedItems(item);
                    }
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryBDThread::singleDownLoadFinished()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());

    M_LOGGER_INFO(QString("%1 singleDownLoadFinished").arg(getClassName()));
    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo
    m_interrupt = false;

    if(reply && m_manager &&reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["error_code"].toInt() == 22000 && value.contains("songinfo"))
            {
                value = value["songinfo"].toMap();
                MusicObject::MusicSongInformation musicInfo;
                musicInfo.m_singerName = value["author"].toString();
                musicInfo.m_songName = value["title"].toString();
                musicInfo.m_timeLength = "-";

                musicInfo.m_songId = value["song_id"].toString();
                musicInfo.m_artistId = value["ting_uid"].toString();
                musicInfo.m_albumId = value["album_id"].toString();

                musicInfo.m_lrcUrl = value["lrclink"].toString();
                musicInfo.m_smallPicUrl = value["pic_small"].toString().replace(",w_90", ",w_500");
                musicInfo.m_albumName = value["album_title"].toString();

                if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                readFromMusicSongAttribute(&musicInfo, value["all_rate"].toString(), m_searchQuality, true);
                if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                if(!musicInfo.m_songAttrs.isEmpty())
                {
                    musicInfo.m_timeLength = findTimeStringByAttrs(musicInfo.m_songAttrs);
                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_albumName = musicInfo.m_albumName;
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
    M_LOGGER_INFO(QString("%1 singleDownLoadFinished deleteAll").arg(getClassName()));
}
