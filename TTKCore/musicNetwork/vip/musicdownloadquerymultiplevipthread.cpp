#include "musicdownloadquerymultiplevipthread.h"
#include "musicdownloadthreadabstract.h"
#include "musicsettingmanager.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>

MusicDownLoadQueryMultipleVipThread::MusicDownLoadQueryMultipleVipThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryMultipleVipThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryMultipleVipThread::startSearchSong(QueryType type, const QString &text)
{
    m_searchText = text.trimmed();
    m_currentType = type;

    QUrl musicUrl = QString(getCurrentURL()).arg(text);
    ///This is a multiple music API

    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Token", "f389fc507e71499ba5d34bce00f43461");
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

QString MusicDownLoadQueryMultipleVipThread::getCurrentURL() const
{
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  return MusicCryptographicHash::decryptData(MUSIC_REQUERY_MULTI_VIP_WY, URL_KEY);
        case 2:  return MusicCryptographicHash::decryptData(MUSIC_REQUERY_MULTI_VIP_QQ, URL_KEY);
        case 3:  return MusicCryptographicHash::decryptData(MUSIC_REQUERY_MULTI_VIP_XM, URL_KEY);
        case 4:  return MusicCryptographicHash::decryptData(MUSIC_REQUERY_MULTI_VIP_TT, URL_KEY);
        case 5:  return MusicCryptographicHash::decryptData(MUSIC_REQUERY_MULTI_VIP_BD, URL_KEY);
        case 6:  return MusicCryptographicHash::decryptData(MUSIC_REQUERY_MULTI_VIP_KW, URL_KEY);
        case 7:  return MusicCryptographicHash::decryptData(MUSIC_REQUERY_MULTI_VIP_KG, URL_KEY);
    }
    return QString();
}

void MusicDownLoadQueryMultipleVipThread::readFromMusicSongAttribute(MusicObject::MusicSongInfomation &info,
                                             const QString &size, int bit, const QString &url)
{
    if(!url.isEmpty())
    {
        QStringList list = url.split("/");
        if(list.isEmpty())
        {
            return;
        }

        QString lastString = list.last();
        MusicObject::MusicSongAttribute songAttr;
        songAttr.m_url = url;
        songAttr.m_size = size.isEmpty() ? "- " : size;

        list = lastString.split(".");
        if(list.isEmpty())
        {
            return;
        }

        lastString = list.last();
        if(lastString.contains("?"))
        {
            lastString = lastString.split("?").front();
        }
        songAttr.m_format = lastString;
        songAttr.m_bitrate = bit;
        info.m_songAttrs << songAttr;
    }
}

void MusicDownLoadQueryMultipleVipThread::downLoadFinished()
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
        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantList datas = data.toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                QVariantMap value = var.toMap();
                MusicObject::MusicSongInfomation musicInfo;
                if(m_currentType != MovieQuery)
                {
                    QString songName = value["songName"].toString();
                    QString singerName = value["artistName"].toString();
                    QString duration = MusicTime::msecTime2LabelJustified(value["length"].toInt()*1000);
                    QString size = value["size"].toString();

                    if(m_queryAllRecords)
                    {
                        readFromMusicSongAttribute(musicInfo, size, MB_1000, value["flacUrl"].toString());
                        readFromMusicSongAttribute(musicInfo, size, MB_1000, value["apeUrl"].toString());
                        readFromMusicSongAttribute(musicInfo, size, MB_1000, value["wavUrl"].toString());
                        readFromMusicSongAttribute(musicInfo, size, MB_320, value["sqUrl"].toString());
                        readFromMusicSongAttribute(musicInfo, size, MB_192, value["hqUrl"].toString());
                        readFromMusicSongAttribute(musicInfo, size, MB_128, value["lqUrl"].toString());
                    }
                    else
                    {
                        if(m_searchQuality == tr("SD"))
                            readFromMusicSongAttribute(musicInfo, size, MB_128, value["lqUrl"].toString());
                        else if(m_searchQuality == tr("HD"))
                            readFromMusicSongAttribute(musicInfo, size, MB_192, value["hqUrl"].toString());
                        else if(m_searchQuality == tr("SQ"))
                            readFromMusicSongAttribute(musicInfo, size, MB_320, value["sqUrl"].toString());
                        else if(m_searchQuality == tr("CD"))
                        {
                            readFromMusicSongAttribute(musicInfo, size, MB_1000, value["flacUrl"].toString());
                            readFromMusicSongAttribute(musicInfo, size, MB_1000, value["apeUrl"].toString());
                            readFromMusicSongAttribute(musicInfo, size, MB_1000, value["wavUrl"].toString());
                        }
                    }
                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }
                    emit createSearchedItems(songName, singerName, duration);

                    musicInfo.m_songId = value["songId"].toString();
                    musicInfo.m_albumId = value["albumId"].toString();
                    musicInfo.m_artistId = value["artistId"].toString();
                    musicInfo.m_songName = songName;
                    musicInfo.m_singerName = singerName;
                    musicInfo.m_timeLength = duration;
                    musicInfo.m_lrcUrl = value["lrcUrl"].toString();
                    musicInfo.m_smallPicUrl = value["picUrl"].toString();
                    m_musicSongInfos << musicInfo;
                }
                else
                {
                    QString songName = value["songName"].toString();
                    QString singerName = value["artistName"].toString();
                    QString duration = MusicTime::msecTime2LabelJustified(value["length"].toInt()*1000);
                    QString size = value["size"].toString();

                    readFromMusicSongAttribute(musicInfo, size, MB_750, value["mvHdUrl"].toString());
                    readFromMusicSongAttribute(musicInfo, size, MB_500, value["mvLdUrl"].toString());

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }
                    emit createSearchedItems(songName, singerName, duration);

                    musicInfo.m_songId = value["songId"].toString();
                    musicInfo.m_albumId = value["albumId"].toString();
                    musicInfo.m_artistId = value["artistId"].toString();
                    musicInfo.m_songName = songName;
                    musicInfo.m_singerName = singerName;
                    musicInfo.m_timeLength = duration;
                    m_musicSongInfos << musicInfo;
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
            M_LOGGER_ERROR("not find the song");
        }
    }
    emit downLoadDataChanged(QString());
    deleteAll();
}
