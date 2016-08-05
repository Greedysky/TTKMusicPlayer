#include "musicdownloadquerymultiplethread.h"
#include "musicdownloadthreadabstract.h"
#include "musicsettingmanager.h"

#ifdef MUSIC_GREATER_NEW
#   include <QJsonArray>
#   include <QJsonObject>
#   include <QJsonValue>
#   include <QJsonParseError>
#else
#   ///QJson import
#   include "qjson/parser.h"
#endif
#include <QNetworkRequest>
#include <QNetworkAccessManager>

MusicDownLoadQueryMultipleThread::MusicDownLoadQueryMultipleThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

MusicDownLoadQueryMultipleThread::~MusicDownLoadQueryMultipleThread()
{

}

QString MusicDownLoadQueryMultipleThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryMultipleThread::startSearchSong(QueryType type, const QString &text)
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

QString MusicDownLoadQueryMultipleThread::getCurrentURL() const
{
    int index = M_SETTING_PTR->value(MusicSettingManager::DownloadServerChoiced).toInt();
    switch( index )
    {
        case 0:  return MUSIC_REQUERY_WY;
        case 1:  return MUSIC_REQUERY_DX;
        case 2:  return MUSIC_REQUERY_QQ;
        case 3:  return MUSIC_REQUERY_XM;
        case 4:  return MUSIC_REQUERY_TT;
        case 5:  return MUSIC_REQUERY_BD;
        case 6:  return MUSIC_REQUERY_KW;
        case 7:  return MUSIC_REQUERY_KG;
        case 8:  return MUSIC_REQUERY_DM;
        case 9:  return MUSIC_REQUERY_MG;
        case 10: return MUSIC_REQUERY_MU;
        case 11: return MUSIC_REQUERY_EC;
        case 12: return MUSIC_REQUERY_YY;
    }
    return QString();
}

void MusicDownLoadQueryMultipleThread::readFromMusicSongAttribute(MusicObject::MusicSongInfomation &info,
                                             const QString &size, int bit,
                                             const QString &url)
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

void MusicDownLoadQueryMultipleThread::downLoadFinished()
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
#ifdef MUSIC_GREATER_NEW
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request
        QJsonParseError jsonError;
        QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
        ///Put the data into Json
        if( jsonError.error != QJsonParseError::NoError )
        {
            emit downLoadDataChanged(QString());
            deleteAll();
            return;
        }

        foreach(QJsonValue value, parseDoucment.array())
        {
            if(!value.isObject())
            {
               continue;
            }
            QJsonObject object = value.toObject();

            MusicObject::MusicSongInfomation musicInfo;
            if(m_currentType != MovieQuery)
            {
                QString songName = object.take("SongName").toString();
                QString singerName = object.take("Artist").toString();
                QString duration = object.take("Length").toString();
                QString size = object.take("Size").toString();

                if(m_queryAllRecords)
                {
                    readFromMusicSongAttribute(musicInfo, size, MB_1000, object.take("FlacUrl").toString());
                    readFromMusicSongAttribute(musicInfo, size, MB_1000, object.take("AacUrl").toString());
                    readFromMusicSongAttribute(musicInfo, size, MB_320, object.take("SqUrl").toString());
                    readFromMusicSongAttribute(musicInfo, size, MB_192, object.take("HqUrl").toString());
                    readFromMusicSongAttribute(musicInfo, size, MB_128, object.take("LqUrl").toString());
                }
                else
                {
                    if(m_searchQuality == tr("SD"))
                        readFromMusicSongAttribute(musicInfo, size, MB_128, object.take("LqUrl").toString());
                    else if(m_searchQuality == tr("HD"))
                        readFromMusicSongAttribute(musicInfo, size, MB_192, object.take("HqUrl").toString());
                    else if(m_searchQuality == tr("SQ"))
                        readFromMusicSongAttribute(musicInfo, size, MB_320, object.take("SqUrl").toString());
                    else if(m_searchQuality == tr("CD"))
                    {
                        readFromMusicSongAttribute(musicInfo, size, MB_1000, object.take("FlacUrl").toString());
                        readFromMusicSongAttribute(musicInfo, size, MB_1000, object.take("AacUrl").toString());
                    }
                }
                if(musicInfo.m_songAttrs.isEmpty())
                {
                    continue;
                }
                emit createSearchedItems(songName, singerName, duration);

                musicInfo.m_songName = songName;
                musicInfo.m_singerName = singerName;
                musicInfo.m_timeLength = duration;
                musicInfo.m_lrcUrl = object.take("LrcUrl").toString();
                musicInfo.m_smallPicUrl = object.take("PicUrl").toString();
                m_musicSongInfos << musicInfo;
            }
            else
            {
                QString songName = object.take("SongName").toString();
                QString singerName = object.take("Artist").toString();
                QString duration = object.take("Length").toString();
                QString size = object.take("Size").toString();

                readFromMusicSongAttribute(musicInfo, size, MB_750, object.take("MvUrl").toString());
                readFromMusicSongAttribute(musicInfo, size, MB_500, object.take("VideoUrl").toString());

                if(musicInfo.m_songAttrs.isEmpty())
                {
                    continue;
                }
                emit createSearchedItems(songName, singerName, duration);

                musicInfo.m_songName = songName;
                musicInfo.m_singerName = singerName;
                musicInfo.m_timeLength = duration;
                m_musicSongInfos << musicInfo;
            }
        }
#else
        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantList datas = data.toList();
            foreach(QVariant var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                QVariantMap value = var.toMap();
                MusicObject::MusicSongInfomation musicInfo;
                if(m_currentType != MovieQuery)
                {
                    QString songName = value["SongName"].toString();
                    QString singerName = value["Artist"].toString();
                    QString duration = value["Length"].toString();
                    QString size = value["Size"].toString();

                    if(m_queryAllRecords)
                    {
                        readFromMusicSongAttribute(musicInfo, size, MB_1000, value["FlacUrl"].toString());
                        readFromMusicSongAttribute(musicInfo, size, MB_1000, value["AacUrl"].toString());
                        readFromMusicSongAttribute(musicInfo, size, MB_320, value["SqUrl"].toString());
                        readFromMusicSongAttribute(musicInfo, size, MB_192, value["HqUrl"].toString());
                        readFromMusicSongAttribute(musicInfo, size, MB_128, value["LqUrl"].toString());
                    }
                    else
                    {
                        if(m_searchQuality == tr("SD"))
                            readFromMusicSongAttribute(musicInfo, size, MB_128, value["LqUrl"].toString());
                        else if(m_searchQuality == tr("HD"))
                            readFromMusicSongAttribute(musicInfo, size, MB_192, value["HqUrl"].toString());
                        else if(m_searchQuality == tr("SQ"))
                            readFromMusicSongAttribute(musicInfo, size, MB_320, value["SqUrl"].toString());
                        else if(m_searchQuality == tr("CD"))
                        {
                            readFromMusicSongAttribute(musicInfo, size, MB_1000, value["FlacUrl"].toString());
                            readFromMusicSongAttribute(musicInfo, size, MB_1000, value["AacUrl"].toString());
                        }
                    }
                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }
                    emit createSearchedItems(songName, singerName, duration);

                    musicInfo.m_songName = songName;
                    musicInfo.m_singerName = singerName;
                    musicInfo.m_timeLength = duration;
                    musicInfo.m_lrcUrl = value["LrcUrl"].toString();
                    musicInfo.m_smallPicUrl = value["PicUrl"].toString();
                    m_musicSongInfos << musicInfo;
                }
                else
                {
                    QString songName = value["SongName"].toString();
                    QString singerName = value["Artist"].toString();
                    QString duration = value["Length"].toString();
                    QString size = value["Size"].toString();

                    readFromMusicSongAttribute(musicInfo, size, MB_750, value["MvUrl"].toString());
                    readFromMusicSongAttribute(musicInfo, size, MB_500, value["VideoUrl"].toString());

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }
                    emit createSearchedItems(songName, singerName, duration);

                    musicInfo.m_songName = songName;
                    musicInfo.m_singerName = singerName;
                    musicInfo.m_timeLength = duration;
                    m_musicSongInfos << musicInfo;
                }
            }
        }
#endif
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
