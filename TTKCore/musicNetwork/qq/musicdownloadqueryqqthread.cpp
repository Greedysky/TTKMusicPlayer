#include "musicdownloadqueryqqthread.h"
#include "musicnumberutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryQQThread::MusicDownLoadQueryQQThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "QQ";
}

QString MusicDownLoadQueryQQThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryQQThread::startSearchSong(QueryType type, const QString &text)
{
    m_searchText = text.trimmed();
    m_currentType = type;
    QUrl musicUrl = MusicCryptographicHash::decryptData(QQ_SONG_SEARCH_URL, URL_KEY).arg(text).arg(0).arg(50);

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

void MusicDownLoadQueryQQThread::downLoadFinished()
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
        bytes = bytes.replace("callback(", "");
        bytes.chop(1);

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["song"].toMap();
                QVariantList datas = value["list"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInfomation musicInfo;
                    foreach(const QVariant &var, value["singer"].toList())
                    {
                        if(var.isNull())
                        {
                            continue;
                        }
                        QVariantMap name = var.toMap();
                        musicInfo.m_singerName = name["name"].toString();
                        musicInfo.m_artistId = QString::number(name["id"].toULongLong());
                    }
                    musicInfo.m_songName = value["songname"].toString();
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["interval"].toInt()*1000);;

                    if(m_currentType != MovieQuery)
                    {
                        musicInfo.m_songId = value["songid"].toString();
                        musicInfo.m_albumId = value["albummid"].toString();
                        musicInfo.m_lrcUrl = MusicCryptographicHash::decryptData(QQ_SONG_LRC_URL, URL_KEY).arg(musicInfo.m_songId);
                        musicInfo.m_smallPicUrl = MusicCryptographicHash::decryptData(QQ_SONG_PIC_URL, URL_KEY)
                                    .arg(musicInfo.m_albumId.right(2).left(1))
                                    .arg(musicInfo.m_albumId.right(1)).arg(musicInfo.m_albumId);

                        if(m_queryAllRecords)
                        {
                            readFromMusicSongAttribute(&musicInfo, value, MB_128);
                            readFromMusicSongAttribute(&musicInfo, value, MB_192);
                            readFromMusicSongAttribute(&musicInfo, value, MB_320);
                            readFromMusicSongAttribute(&musicInfo, value, MB_500);
                            readFromMusicSongAttribute(&musicInfo, value, MB_1000);
                        }
                        else
                        {
                            if(m_searchQuality == tr("SD"))
                                readFromMusicSongAttribute(&musicInfo, value, MB_128);
                            else if(m_searchQuality == tr("HQ"))
                                readFromMusicSongAttribute(&musicInfo, value, MB_192);
                            else if(m_searchQuality == tr("SQ"))
                                readFromMusicSongAttribute(&musicInfo, value, MB_320);
                            else if(m_searchQuality == tr("CD"))
                            {
                                readFromMusicSongAttribute(&musicInfo, value, MB_500);
                                readFromMusicSongAttribute(&musicInfo, value, MB_1000);
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
