#include "musicdownloadqueryqqrecommendthread.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryQQRecommendThread::MusicDownLoadQueryQQRecommendThread(QObject *parent)
    : MusicDownLoadQueryRecommendThread(parent)
{
    m_queryServer = "QQ";
}

QString MusicDownLoadQueryQQRecommendThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryQQRecommendThread::startToSearch(const QString &id)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(id));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(QQ_RCM_URL, false);
    deleteAll();
    m_searchText = id;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryQQRecommendThread::downLoadFinished()
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
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();
                QVariantList datas = value["slider"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInformation musicInfo;
                    musicInfo.m_songId = QString::number(value["id"].toULongLong());

                    if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    readFromMusicAttribute(&musicInfo);
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

void MusicDownLoadQueryQQRecommendThread::readFromMusicAttribute(MusicObject::MusicSongInformation *info)
{
    if(!m_manager)
    {
        return;
    }

    QUrl musicUrl = MusicUtils::Algorithm::mdII(QQ_SONG_INFO_URL, false).arg(info->m_songId);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = m_manager->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("data") && value["code"].toInt() == 0)
        {
            QVariantList datas = value["data"].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                foreach(const QVariant &var, value["singer"].toList())
                {
                    if(var.isNull())
                    {
                        continue;
                    }
                    QVariantMap name = var.toMap();
                    info->m_singerName = name["name"].toString();
                    info->m_artistId = name["mid"].toString();
                }
                info->m_songName = value["name"].toString();
                info->m_timeLength = MusicTime::msecTime2LabelJustified(value["interval"].toInt()*1000);

                m_rawData["songID"] = QString::number(value["id"].toLongLong());
                info->m_songId = value["mid"].toString();

                QVariantMap albumMap = value["album"].toMap();
                info->m_albumName = albumMap["name"].toString();
                info->m_albumId = albumMap["mid"].toString();

                info->m_lrcUrl = MusicUtils::Algorithm::mdII(QQ_SONG_LRC_URL, false).arg(info->m_songId);
                info->m_smallPicUrl = MusicUtils::Algorithm::mdII(QQ_SONG_PIC_URL, false)
                            .arg(info->m_albumId.right(2).left(1))
                            .arg(info->m_albumId.right(1)).arg(info->m_albumId);

                if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                readFromMusicSongAttributeInfo(info, value["file"].toMap());
                if(m_interrupt || !m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
            }
        }
    }
}
