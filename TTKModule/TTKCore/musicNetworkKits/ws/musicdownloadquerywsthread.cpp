#include "musicdownloadquerywsthread.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryWSThread::MusicDownLoadQueryWSThread(const QString &type, QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "WuSing";
    m_querySearchType = type;
}

QString MusicDownLoadQueryWSThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryWSThread::startToSearch(QueryType type, const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    m_searchText = text.trimmed();
    m_currentType = type;
    m_interrupt = true;

    int ty = 1;
    if(m_querySearchType == "yc") ty = 1;
    else if(m_querySearchType == "fc") ty = 2;
    else if(m_querySearchType == "bz") ty = 3;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WS_SONG_SEARCH_URL, false).arg(0).arg(ty).arg(text).arg(1).arg(40);
    deleteAll();

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(WS_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryWSThread::downLoadFinished()
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
        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("code") && value["code"].toString() == "0000000")
            {
                value = value["data"].toMap();
                QVariantList datas = value["songArray"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_songName = value["songName"].toString();
                    musicInfo.m_songId = QString::number(value["songId"].toInt());
                    musicInfo.m_lrcUrl = "-";
                    musicInfo.m_timeLength = "-";

                    musicInfo.m_artistId = QString::number(value["singerId"].toULongLong());
                    musicInfo.m_singerName = value["singer"].toString();

                    if(!m_querySimplify)
                    {
                        if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                        readFromMusicSongAttribute(&musicInfo, m_querySearchType, m_searchQuality, m_queryAllRecords);
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
