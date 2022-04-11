#include "musicwyqueryalbumrequest.h"

#include <QDateTime>

MusicWYQueryAlbumRequest::MusicWYQueryAlbumRequest(QObject *parent)
    : MusicQueryAlbumRequest(parent)
{
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryAlbumRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();
    m_queryValue = value;

    QNetworkRequest request;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(WY_ALBUM_URL, false).arg(value),
                      QString("{}"));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicWYQueryAlbumRequest::startToSingleSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSingleSearch %2").arg(className(), value));

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(WY_ARTIST_ALBUM_URL, false).arg(value),
                      MusicUtils::Algorithm::mdII(WY_ARTIST_ALBUM_DATA_URL, false));

    QNetworkReply *reply = m_manager.post(request, parameter);
    connect(reply, SIGNAL(finished()), SLOT(downLoadSingleFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicWYQueryAlbumRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicQueryAlbumRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("album"))
            {
                bool albumFound = false;
                //
                MusicResultsItem info;
                const QVariantMap &albumValue = value["album"].toMap();
                info.m_coverUrl = albumValue["picUrl"].toString();
                info.m_description = albumValue["name"].toString() + TTK_SPLITER +
                                     albumValue["language"].toString() + TTK_SPLITER +
                                     albumValue["company"].toString() + TTK_SPLITER +
                                     QDateTime::fromMSecsSinceEpoch(albumValue["publishTime"].toULongLong()).toString(MUSIC_YEAR_FORMAT);
                //
                const QVariantList &datas = value["songs"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_songName = MusicUtils::String::charactersReplaced(value["name"].toString());
                    musicInfo.m_duration = MusicTime::msecTime2LabelJustified(value["dt"].toInt());
                    musicInfo.m_songId = QString::number(value["id"].toInt());
                    musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(musicInfo.m_songId);

                    const QVariantMap &albumObject = value["al"].toMap();
                    musicInfo.m_coverUrl = albumObject["picUrl"].toString();
                    musicInfo.m_albumId = QString::number(albumObject["id"].toInt());
                    musicInfo.m_albumName = MusicUtils::String::charactersReplaced(albumObject["name"].toString());

                    const QVariantList &artistsArray = value["ar"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artistObject = artistValue.toMap();
                        musicInfo.m_artistId = QString::number(artistObject["id"].toULongLong());
                        musicInfo.m_singerName = MusicUtils::String::charactersReplaced(artistObject["name"].toString());
                        break; //just find first singer
                    }

                    musicInfo.m_year = QString();
                    musicInfo.m_discNumber = value["cd"].toString();
                    musicInfo.m_trackNumber = value["no"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    readFromMusicSongPropertyNew(&musicInfo, value, m_queryQuality, m_queryAllRecords);
                    TTK_NETWORK_QUERY_CHECK();

                    if(musicInfo.m_songProps.isEmpty())
                    {
                        continue;
                    }
                    //
                    if(!albumFound)
                    {
                        albumFound = true;
                        info.m_id = musicInfo.m_albumId;
                        info.m_name = musicInfo.m_singerName;
                        Q_EMIT createAlbumInfoItem(info);
                    }
                    //
                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_albumName = musicInfo.m_albumName;
                    item.m_duration = musicInfo.m_duration;
                    item.m_type = mapQueryServerString();
                    Q_EMIT createSearchedItem(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicWYQueryAlbumRequest::downLoadSingleFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadSingleFinished").arg(className()));

    MusicPageQueryRequest::downLoadFinished();
    QNetworkReply *reply = TTKObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("hotAlbums") && value["code"].toInt() == 200)
            {
                const QVariantList &datas = value["hotAlbums"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem info;
                    info.m_id = QString::number(value["id"].toULongLong());
                    info.m_coverUrl = value["picUrl"].toString();
                    info.m_name = value["name"].toString();
                    info.m_updateTime = QDateTime::fromMSecsSinceEpoch(value["publishTime"].toULongLong()).toString("yyyy.MM.dd");
                    Q_EMIT createAlbumInfoItem(info);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
