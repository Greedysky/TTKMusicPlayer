#include "musicdownloadqueryttthread.h"
#include "musicdownloadthreadabstract.h"
#include "musicnumberutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>

MusicDownLoadQueryTTThread::MusicDownLoadQueryTTThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryTTThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryTTThread::startSearchSong(QueryType type, const QString &text)
{
    m_searchText = text.trimmed();
    m_currentType = type;

    QUrl musicUrl = MusicCryptographicHash::decryptData(TT_SEARCH_URL, URL_KEY).arg(text);

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

void MusicDownLoadQueryTTThread::downLoadFinished()
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

                        musicInfo.m_songId = songId;
                        musicInfo.m_albumId = QString::number(value["albumId"].toULongLong());
                        musicInfo.m_lrcUrl = MusicCryptographicHash::decryptData(TT_SONG_LRC_URL, URL_KEY).arg(singerName).arg(songName).arg(songId);
                        musicInfo.m_smallPicUrl = value["picUrl"].toString();
                        musicInfo.m_singerName = singerName;
                        musicInfo.m_songName = songName;
                        musicInfo.m_timeLength = duration;
                        m_musicSongInfos << musicInfo;
                    }
                    else
                    {
                        QVariantList mvs = value["mvList"].toList();
                        if(!mvs.isEmpty())
                        {
                            QString duration;
                            foreach(const QVariant &mv, mvs)
                            {
                                QVariantMap mvUrlValue = mv.toMap();
                                if(mvUrlValue.isEmpty())
                                {
                                    continue;
                                }

                                int bitRate = mvUrlValue["bitRate"].toInt();
                                if(bitRate == 0)
                                {
                                    continue;
                                }

                                duration = MusicTime::msecTime2LabelJustified(mvUrlValue["duration"].toInt());
                                MusicObject::MusicSongAttribute songAttr;

                                if(bitRate > 375 && bitRate <= 625)
                                    songAttr.m_bitrate = MB_500;
                                else if(bitRate > 625 && bitRate <= 875)
                                    songAttr.m_bitrate = MB_750;
                                else if(bitRate > 875)
                                    songAttr.m_bitrate = MB_1000;
                                else
                                    songAttr.m_bitrate = bitRate;

                                songAttr.m_format = mvUrlValue["suffix"].toString();
                                songAttr.m_url = mvUrlValue["url"].toString();
                                songAttr.m_size = MusicUtils::Number::size2Label(mvUrlValue["size"].toInt());
                                musicInfo.m_songAttrs << songAttr;
                            }
                            emit createSearchedItems(songName, singerName, duration);

                            musicInfo.m_singerName = singerName;
                            musicInfo.m_songName = songName;
                            m_musicSongInfos << musicInfo;
                        }
                    }
                }
            }
        }
        ///If there is no search to song_id, is repeated several times in the search
        ///If more than 5 times or no results give up
        static int counter = 5;
        if(m_musicSongInfos.isEmpty() && counter-- > 0)
        {
            startSearchSong(m_currentType, m_searchText);
        }
        else
        {
            M_LOGGER_ERROR("not find the song_Id");
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
