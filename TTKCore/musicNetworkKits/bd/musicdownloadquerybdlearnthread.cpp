#include "musicdownloadquerybdlearnthread.h"
#include "musicdownloadbdinterface.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"
#include "qaeswrap.h"

MusicDownLoadQueryBDLearnThread::MusicDownLoadQueryBDLearnThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "Baidu";
}

QString MusicDownLoadQueryBDLearnThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryBDLearnThread::startToSearch(QueryType type, const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    Q_UNUSED(type);
    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_SG_LEAEN_URL, false).arg(text).arg(1).arg(30);
    deleteAll();

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
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

void MusicDownLoadQueryBDLearnThread::downLoadFinished()
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
            if(value["error_code"].toInt() == 22000 && value.contains("result"))
            {
                value = value["result"].toMap();
                QVariantList datas = value["items"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInfomation musicInfo;
                    musicInfo.m_singerName = value["artist_name"].toString();
                    musicInfo.m_songName = value["song_title"].toString();
                    musicInfo.m_songId = value["song_id"].toString();
                    musicInfo.m_albumId = value["album_id"].toString();

                    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    readFromMusicLrcAttribute(&musicInfo);
                    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
                    readFromMusicSongAttribute(&musicInfo);
                    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;

                    if(musicInfo.m_songAttrs.isEmpty())
                    {
                        continue;
                    }

                    MusicSearchedItem item;
                    item.m_songname = musicInfo.m_songName;
                    item.m_artistname = musicInfo.m_singerName;
                    item.m_type = mapQueryServerString();
                    emit createSearchedItems(item);
                    m_musicSongInfos << musicInfo;
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}

void MusicDownLoadQueryBDLearnThread::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info)
{
    if(!m_manager)
    {
        return;
    }

    QString key = MusicUtils::Algorithm::mdII(BD_SG_LEAEN_PA_URL, false).arg(info->m_songId)
                  .arg(MusicTime::timeStamp());
    QString eKey = QString(QAesWrap::encrypt(key.toUtf8(), "4CC20A0C44FEB6FD", "2012061402992850"));
    eKey.replace('+', "%2B");
    eKey.replace('/', "%2F");
    eKey.replace('=', "%3D");
    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_SG_LEAEN_INFO_URL, false).arg(key).arg(eKey);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
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

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["error_code"].toInt() == 22000 && value.contains("result"))
        {
            value = value["result"].toMap();
            MusicObject::MusicSongAttribute attr;
            attr.m_url = value["merge_link"].toString();
            info->m_songAttrs.append(attr);
        }
    }
}

void MusicDownLoadQueryBDLearnThread::readFromMusicLrcAttribute(MusicObject::MusicSongInfomation *info)
{
    if(!m_manager)
    {
        return;
    }

    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_SONG_FMINFO_URL, false).arg(info->m_songId);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
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

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["errorCode"].toInt() == 22000 && value.contains("data"))
        {
            value = value["data"].toMap();
            QVariantList datas = value["songList"].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                info->m_lrcUrl = value["lrcLink"].toString();
            }
        }
    }
}
