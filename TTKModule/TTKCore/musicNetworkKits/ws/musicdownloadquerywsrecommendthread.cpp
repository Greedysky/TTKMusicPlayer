#include "musicdownloadquerywsrecommendthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryWSRecommendThread::MusicDownLoadQueryWSRecommendThread(QObject *parent)
    : MusicDownLoadQueryRecommendThread(parent)
{
    m_queryServer = "WuSing";
}

QString MusicDownLoadQueryWSRecommendThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryWSRecommendThread::startToSearch(const QString &id)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(id));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WS_RCM_URL, false).arg(50);
    m_interrupt = true;

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

void MusicDownLoadQueryWSRecommendThread::downLoadFinished()
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
        QByteArray bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(!value.isEmpty() && value.contains("data"))
            {
                value = value["data"].toMap();
                QVariantList datas = value["list"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_songName = "-";
                    musicInfo.m_songId = value["SongId"].toString();
                    musicInfo.m_lrcUrl = "-";
                    musicInfo.m_timeLength = "-";

                    musicInfo.m_artistId = QString::number(value["UserId"].toULongLong());
                    musicInfo.m_singerName = value["NickName"].toString();

                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    readFromMusicSongAttribute(&musicInfo, value["SongType"].toString(), m_searchQuality, m_queryAllRecords);
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
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
