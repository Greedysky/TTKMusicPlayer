#include "musicdownloadqueryalbumttthread.h"
#include "musicdownloadqueryttthread.h"
#include "musicnumberutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryAlbumTTThread::MusicDownLoadQueryAlbumTTThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryAlbumTTThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryAlbumTTThread::startSearchSong(QueryType type, const QString &text)
{
    Q_UNUSED(type);
    startSearchSong(text);
}

void MusicDownLoadQueryAlbumTTThread::startSearchSong(const QString &album)
{
    QUrl musicUrl = MusicCryptographicHash::decryptData(TT_SONG_ALBUM_URL, URL_KEY).arg(album);

    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    QNetworkRequest request;
    request.setUrl(musicUrl);
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()) );
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     SLOT(replyError(QNetworkReply::NetworkError)) );
}

void MusicDownLoadQueryAlbumTTThread::downLoadFinished()
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
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 1 || value["code"].toInt() == 200)
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

                    QString songId = QString::number(value["songId"].toULongLong());
                    QString songName = value["name"].toString();
                    QString singerName = value["singerName"].toString();

                    if(m_currentType != MovieQuery)
                    {
                        QString duration;
                        ///music normal songs urls
                        QVariantList auditions = value["auditionList"].toList();
                        foreach(const QVariant &audition, auditions)
                        {
                            QVariantMap audUrlsValue = audition.toMap();
                            if(audUrlsValue.isEmpty())
                            {
                                continue;
                            }

                            if(m_queryAllRecords == true || (m_queryAllRecords == false &&
                               audUrlsValue["typeDescription"].toString() == m_searchQuality))
                            {
                                MusicObject::MusicSongAttribute songAttr;
                                songAttr.m_url = audUrlsValue["url"].toString();
                                songAttr.m_size = MusicUtils::Number::size2Label(audUrlsValue["size"].toInt());
                                songAttr.m_format = audUrlsValue["suffix"].toString();
                                songAttr.m_bitrate = audUrlsValue["bitRate"].toInt();
                                musicInfo.m_songAttrs << songAttr;
                                ////set duration
                                duration = MusicTime::msecTime2LabelJustified(audUrlsValue["duration"].toInt());
                                if(!m_queryAllRecords)
                                {
                                    break;
                                }
                            }
                        }
                        ///music cd songs urls
                        QVariantList llUrls = value["llList"].toList();
                        foreach(const QVariant &llUrl, llUrls)
                        {
                            QVariantMap llUrlValue = llUrl.toMap();
                            if(llUrlValue.isEmpty())
                            {
                                continue;
                            }

                            if(m_queryAllRecords == true || (m_queryAllRecords == false &&
                               llUrlValue["typeDescription"].toString() == m_searchQuality))
                            {
                                MusicObject::MusicSongAttribute songAttr;
                                songAttr.m_url = llUrlValue["url"].toString();
                                songAttr.m_size = MusicUtils::Number::size2Label(llUrlValue["size"].toInt());
                                songAttr.m_format = llUrlValue["suffix"].toString();
                                songAttr.m_bitrate = llUrlValue["bitRate"].toInt();
                                musicInfo.m_songAttrs << songAttr;
                                ////set duration
                                duration = MusicTime::msecTime2LabelJustified(llUrlValue["duration"].toInt());
                                if(!m_queryAllRecords)
                                {
                                    break;
                                }
                            }
                        }

                        if(musicInfo.m_songAttrs.isEmpty())
                        {
                            continue;
                        }
                        emit createSearchedItems(songName, singerName, duration);

                        musicInfo.m_albumId = value["albumName"].toString() + "<>" +
                                              QString::number(value["lang"].toInt()) + "<>" +
                                              value["company"].toString() + "<>" +
                                              QString::number(value["releaseYear"].toInt());
                        musicInfo.m_lrcUrl = MusicCryptographicHash::decryptData(TT_SONG_LRC_URL, URL_KEY).arg(singerName).arg(songName).arg(songId);
                        musicInfo.m_smallPicUrl = value["picUrl"].toString();
                        musicInfo.m_singerName = singerName;
                        musicInfo.m_songName = songName;
                        musicInfo.m_timeLength = duration;
                        m_musicSongInfos << musicInfo;
                    }
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
