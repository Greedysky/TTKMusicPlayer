#include "musicdownloadquerywyartistthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryWYArtistThread::MusicDownLoadQueryWYArtistThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "WangYi";
}

QString MusicDownLoadQueryWYArtistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryWYArtistThread::startToSearch(QueryType type, const QString &artist)
{
    Q_UNUSED(type);
    startToSearch(artist);
}

void MusicDownLoadQueryWYArtistThread::startToSearch(const QString &artist)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(artist));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WY_ARTIST_URL, false).arg(artist);
    deleteAll();

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
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

void MusicDownLoadQueryWYArtistThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
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
            if(value["code"].toInt() == 200 && value.contains("hotSongs"))
            {
                QVariantMap artistObject = value["artist"].toMap();
                QString smallPicUrl = artistObject["picUrl"].toString();
                QString singerName = artistObject["name"].toString();

                QVariantList datas = value["hotSongs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInfomation musicInfo;
                    musicInfo.m_songName = value["name"].toString();
                    musicInfo.m_singerName = singerName;
                    musicInfo.m_smallPicUrl = smallPicUrl;
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt());
                    musicInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(WY_SONG_LRC_URL, false).arg(value["id"].toInt());

                    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    readFromMusicSongAttribute(&musicInfo, value, m_searchQuality, m_queryAllRecords);
                    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }

                    MusicSearchedItem item;
                    item.m_songname = musicInfo.m_songName;
                    item.m_artistname = musicInfo.m_singerName;
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
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
