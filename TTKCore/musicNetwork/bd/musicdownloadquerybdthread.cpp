#include "musicdownloadquerybdthread.h"
#include "musictime.h"
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

void MusicDownLoadQueryBDThread::startSearchSong(QueryType type, const QString &text)
{
    m_searchText = text.trimmed();
    m_currentType = type;
    QUrl musicUrl = MusicCryptographicHash::decryptData(BD_SONG_SEARCH_URL, URL_KEY).arg(text).arg(0).arg(50);

    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryBDThread::downLoadFinished()
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
                    MusicObject::MusicSongInfomation musicInfo;
                    musicInfo.m_singerName = value["author"].toString();
                    musicInfo.m_songName = value["title"].toString();
                    musicInfo.m_timeLength = "-";

                    if(m_currentType != MovieQuery)
                    {
                        musicInfo.m_songId = value["song_id"].toString();
                        musicInfo.m_artistId = value["ting_uid"].toString();
                        musicInfo.m_albumId = value["album_id"].toString();
                        musicInfo.m_lrcUrl = value["lrclink"].toString();
                        musicInfo.m_smallPicUrl = value["pic_small"].toString().replace(",w_90", ",w_500");

                        QString format = value["all_rate"].toString();
                        foreach(const QString &f, format.split(","))
                        {
                            if(m_queryAllRecords)
                            {
                                if(f != "flac")
                                {
                                    readFromMusicSongAttribute(&musicInfo, m_manager, f, musicInfo.m_songId);
                                }
                                else
                                {
                                    readFromMusicLLAttribute(&musicInfo, m_manager, musicInfo.m_songId);
                                }
                            }
                            else
                            {
                                if(f != "flac")
                                {
                                    int bit = map2NormalBitrate(f.toInt());
                                    if(m_searchQuality == tr("ST") && bit < MB_128)
                                        readFromMusicSongAttribute(&musicInfo, m_manager, f, musicInfo.m_songId);
                                    else if(m_searchQuality == tr("SD") && bit >= MB_128 && bit < MB_192)
                                        readFromMusicSongAttribute(&musicInfo, m_manager, f, musicInfo.m_songId);
                                    else if(m_searchQuality == tr("HQ") && bit >= MB_192 && bit < MB_320)
                                        readFromMusicSongAttribute(&musicInfo, m_manager, f, musicInfo.m_songId);
                                    else if(m_searchQuality == tr("SQ") && bit >= MB_320)
                                        readFromMusicSongAttribute(&musicInfo, m_manager, f, musicInfo.m_songId);
                                }
                                else if(m_searchQuality == tr("CD"))
                                {
                                    readFromMusicLLAttribute(&musicInfo, m_manager, musicInfo.m_songId);
                                }
                            }
                        }

                        if(musicInfo.m_songAttrs.isEmpty())
                        {
                            continue;
                        }

                        emit createSearchedItems(musicInfo.m_songName, musicInfo.m_singerName, musicInfo.m_timeLength);
                        m_musicSongInfos << musicInfo;
                    }
                    else
                    {
                        //MV
                    }
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
