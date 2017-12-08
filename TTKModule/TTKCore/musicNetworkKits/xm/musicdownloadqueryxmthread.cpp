#include "musicdownloadqueryxmthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryXMThread::MusicDownLoadQueryXMThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "XiaMi";
}

QString MusicDownLoadQueryXMThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryXMThread::startToSearch(QueryType type, const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    m_searchText = text.trimmed();
    m_currentType = type;
    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(XM_SONG_DATA_URL, false).arg(text).arg(1).arg(30),
                      MusicUtils::Algorithm::mdII(XM_SONG_URL, false));
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryXMThread::startToSingleSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSingleSearch %2").arg(getClassName()).arg(text));
    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(XM_SONG_DATA_INFO_URL, false).arg(text),
                      MusicUtils::Algorithm::mdII(XM_SONG_INFO_URL, false));
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(singleDownLoadFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryXMThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo
    m_interrupt = false;

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
                value = value["data"].toMap();
                QVariantList datas = value["songs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_singerName = value["singers"].toString();
                    musicInfo.m_songName = value["songName"].toString();
                    musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["length"].toInt());

                    musicInfo.m_songId = value["songId"].toString();
                    musicInfo.m_artistId = value["artistId"].toString();
                    musicInfo.m_albumId = value["albumId"].toString();
                    if(!m_querySimplify)
                    {
                        musicInfo.m_smallPicUrl = value["albumLogo"].toString();
                        musicInfo.m_albumName = value["albumName"].toString();

                        if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        readFromMusicSongLrc(&musicInfo);
                        if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        readFromMusicSongAttribute(&musicInfo, value["listenFiles"], m_searchQuality, m_queryAllRecords);
                        if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                        if(musicInfo.m_songAttrs.isEmpty())
                        {
                            continue;
                        }

                        MusicSearchedItem item;
                        item.m_songName = musicInfo.m_songName;
                        item.m_singerName = musicInfo.m_singerName;
                        item.m_albumName = musicInfo.m_albumName;
                        item.m_time = musicInfo.m_timeLength;
                        item.m_type = mapQueryServerString();
                        emit createSearchedItems(item);
                    }
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}

void MusicDownLoadQueryXMThread::singleDownLoadFinished()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());

    M_LOGGER_INFO(QString("%1 singleDownLoadFinished").arg(getClassName()));
    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo
    m_interrupt = false;

    if(reply && m_manager &&reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["data"].toMap();
                value = value["songDetail"].toMap();

                MusicObject::MusicSongInformation musicInfo;
                musicInfo.m_singerName = value["singers"].toString();
                musicInfo.m_songName = value["songName"].toString();
                musicInfo.m_timeLength = MusicTime::msecTime2LabelJustified(value["length"].toInt());

                musicInfo.m_songId = value["songId"].toString();
                musicInfo.m_artistId = value["artistId"].toString();
                musicInfo.m_albumId = value["albumId"].toString();

                musicInfo.m_smallPicUrl = value["albumLogo"].toString();
                musicInfo.m_albumName = value["albumName"].toString();

                if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                readFromMusicSongLrc(&musicInfo);
                if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                readFromMusicSongAttribute(&musicInfo, value["listenFiles"], m_searchQuality, m_queryAllRecords);
                if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                if(!musicInfo.m_songAttrs.isEmpty())
                {
                    MusicSearchedItem item;
                    item.m_songName = musicInfo.m_songName;
                    item.m_singerName = musicInfo.m_singerName;
                    item.m_albumName = musicInfo.m_albumName;
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
    M_LOGGER_INFO(QString("%1 singleDownLoadFinished deleteAll").arg(getClassName()));
}
