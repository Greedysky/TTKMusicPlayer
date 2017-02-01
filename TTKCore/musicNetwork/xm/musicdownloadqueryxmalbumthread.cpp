#include "musicdownloadqueryxmalbumthread.h"
#include "musicdownloadqueryxmthread.h"
#include "musicnumberutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryXMAlbumThread::MusicDownLoadQueryXMAlbumThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryXMAlbumThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryXMAlbumThread::startSearchSong(QueryType type, const QString &album)
{
    Q_UNUSED(type);
    startSearchSong(album);
}

void MusicDownLoadQueryXMAlbumThread::startSearchSong(const QString &album)
{
    QUrl musicUrl = MusicCryptographicHash::decryptData(XM_SONG_ALBUM_URL, URL_KEY).arg(album);

    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicCryptographicHash::decryptData(XM_BASE_URL, URL_KEY).toUtf8());
    request.setRawHeader("Referer", MusicCryptographicHash::decryptData(XM_BASE_URL, URL_KEY).toUtf8());
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

void MusicDownLoadQueryXMAlbumThread::downLoadFinished()
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
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                QVariantList datas = value["trackList"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    if(value["song_status"].toInt() != 0)
                    {
                        continue;
                    }

                    MusicObject::MusicSongInfomation musicInfo;
                    musicInfo.m_singerName = value["singers"].toString();
                    musicInfo.m_songName = value["songName"].toString();
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["length"].toInt()*1000);

                    if(m_currentType != MovieQuery)
                    {
                        musicInfo.m_songId = QString::number(value["songId"].toULongLong());
                        musicInfo.m_artistId = QString::number(value["artistId"].toULongLong());
                        musicInfo.m_albumId = value["name"].toString() + "<>" +
                                            value["language"].toString() + "<>" +
                                            value["company"].toString() + "<>" +
                                QDateTime::fromMSecsSinceEpoch(value["demoCreateTime"].toULongLong()/1000)
                                             .toString("yyyy-MM-dd");

                        musicInfo.m_smallPicUrl = MusicCryptographicHash::decryptData(XM_SONG_PIC_URL, URL_KEY) +
                                                  value["album_logo"].toString().replace("_1.", "_4.");
                        musicInfo.m_lrcUrl = value["lyric_url"].toString();

                        ///music normal songs urls
                        QVariantList auditions = value["allAudios"].toList();
                        foreach(const QVariant &audition, auditions)
                        {
                            QVariantMap audUrlsValue = audition.toMap();
                            if(audUrlsValue.isEmpty())
                            {
                                continue;
                            }

                            int bitrate = map2NormalBitrate(audUrlsValue["rate"].toInt());
                            if(m_queryAllRecords)
                            {
                                readFromMusicSongAttribute(&musicInfo, audUrlsValue, bitrate);
                            }
                            else
                            {
                                if(m_searchQuality == tr("ST") && bitrate == MB_32)
                                    readFromMusicSongAttribute(&musicInfo, audUrlsValue, MB_32);
                                else if(m_searchQuality == tr("SD") && bitrate == MB_128)
                                    readFromMusicSongAttribute(&musicInfo, audUrlsValue, MB_128);
                                else if(m_searchQuality == tr("HD") && bitrate == MB_192)
                                    readFromMusicSongAttribute(&musicInfo, audUrlsValue, MB_192);
                                else if(m_searchQuality == tr("SQ") && bitrate == MB_320)
                                    readFromMusicSongAttribute(&musicInfo, audUrlsValue, MB_320);
                                else if(m_searchQuality == tr("CD") && bitrate == MB_500)
                                    readFromMusicSongAttribute(&musicInfo, audUrlsValue, MB_500);
                            }
                        }

                        if(musicInfo.m_songAttrs.isEmpty())
                        {
                            continue;
                        }

                        emit createSearchedItems(musicInfo.m_songName, musicInfo.m_singerName, musicInfo.m_timeLength);
                        m_musicSongInfos << musicInfo;
                    }
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}

void MusicDownLoadQueryXMAlbumThread::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                const QVariantMap &key, int bitrate)
{
    MusicObject::MusicSongAttribute attr;
    attr.m_url = key["filePath"].toString();
    attr.m_size = MusicUtils::Number::size2Label(key["fileSize"].toInt());
    attr.m_format = key["format"].toString();
    attr.m_bitrate = bitrate;
    info->m_songAttrs.append(attr);
}

int MusicDownLoadQueryXMAlbumThread::map2NormalBitrate(int bitrate)
{
    if(bitrate > 0 && bitrate < 128)
        return MB_32;
    else if(bitrate > 128 && bitrate < 192)
        return MB_128;
    else if(bitrate > 192 && bitrate < 320)
        return MB_320;
    else if(bitrate > 320)
        return MB_500;
    else
        return bitrate;
}
