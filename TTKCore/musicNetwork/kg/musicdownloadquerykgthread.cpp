#include "musicdownloadquerykgthread.h"
#include "musicsemaphoreloop.h"
#include "musicnumberutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryKGThread::MusicDownLoadQueryKGThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "Kugou";
}

QString MusicDownLoadQueryKGThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryKGThread::startSearchSong(QueryType type, const QString &text)
{
    m_searchText = text.trimmed();
    m_currentType = type;
    QUrl musicUrl = MusicCryptographicHash::decryptData(KG_SONG_SEARCH_URL, URL_KEY).arg(text).arg(0).arg(50);

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

void MusicDownLoadQueryKGThread::downLoadFinished()
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
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                QVariantList datas = value["info"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInfomation musicInfo;
                    musicInfo.m_singerName = value["singername"].toString();
                    musicInfo.m_songName = value["songname"].toString();
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt()*1000);

                    if(m_currentType != MovieQuery)
                    {
                        musicInfo.m_songId = value["hash"].toString();
                        musicInfo.m_albumId = value["album_id"].toString();

                        readFromMusicSongLrcAndPic(&musicInfo, value["hash"].toString(), m_manager);

                        if(m_queryAllRecords)
                        {
                            readFromMusicSongAttribute(&musicInfo, value["hash"].toString(), m_manager);
                            readFromMusicSongAttribute(&musicInfo, value["320hash"].toString(), m_manager);
                            readFromMusicSongAttribute(&musicInfo, value["sqhash"].toString(), m_manager);
                        }
                        else
                        {
                            if(m_searchQuality == tr("SD"))
                                readFromMusicSongAttribute(&musicInfo, value["hash"].toString(), m_manager);
                            else if(m_searchQuality == tr("SQ"))
                                readFromMusicSongAttribute(&musicInfo, value["320hash"].toString(), m_manager);
                            else if(m_searchQuality == tr("CD"))
                                readFromMusicSongAttribute(&musicInfo, value["sqhash"].toString(), m_manager);
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
