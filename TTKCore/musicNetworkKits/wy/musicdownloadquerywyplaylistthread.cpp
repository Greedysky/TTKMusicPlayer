#include "musicdownloadquerywyplaylistthread.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryWYPlaylistThread::MusicDownLoadQueryWYPlaylistThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_queryServer = "WangYi";
}

QString MusicDownLoadQueryWYPlaylistThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryWYPlaylistThread::startSearchSong(QueryType type, const QString &playlist)
{

}

void MusicDownLoadQueryWYPlaylistThread::startSearchSong()
{
    QUrl musicUrl = QString("http://music.163.com/api/playlist/list?cat=all&order=hot&offset=0&total=false&limit=50");

    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
    request.setRawHeader("Referer", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
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

void MusicDownLoadQueryWYPlaylistThread::downLoadFinished()
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
            if(value["code"].toInt() == 200 && value.contains("playlists"))
            {
                QVariantList datas = value["playlists"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicPlaylistItem item;
                    item.m_coverUrl = value["coverImgUrl"].toString();
                    item.m_id = QString::number(value["id"].toULongLong());
                    item.m_name = value["name"].toString();
                    item.m_playCount = QString::number(value["playCount"].toULongLong());
                    item.m_description = value["description"].toString();

                    value = value["creator"].toMap();
                    item.m_nickname = value["nickname"].toString();

                    emit createPlaylistItems(item);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
