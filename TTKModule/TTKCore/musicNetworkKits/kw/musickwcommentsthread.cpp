#include "musickwcommentsthread.h"
#include "musicdownloadquerykwthread.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"

MusicKWSongCommentsThread::MusicKWSongCommentsThread(QObject *parent)
    : MusicDownLoadCommentsThread(parent)
{
    m_pageSize = 20;
}

QString MusicKWSongCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicKWSongCommentsThread::startToSearch(const QString &name)
{
    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));
    MusicSemaphoreLoop loop;
    MusicDownLoadQueryKWThread *query = new MusicDownLoadQueryKWThread(this);
    query->setQueryAllRecords(false);
    query->setQuerySimplify(true);
    query->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, name);
    connect(query, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    m_rawData["songID"].clear();
    if(!query->getMusicSongInfos().isEmpty())
    {
        m_rawData["songID"] = query->getMusicSongInfos().first().m_songId;
        startToPage(0);
    }
}

void MusicKWSongCommentsThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();
    m_pageTotal = 0;
    m_interrupt = true;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KW_SG_COMMIT_URL, false)
                    .arg(m_rawData["songID"].toString()).arg(offset + 1).arg(m_pageSize);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicKWSongCommentsThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
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
            if(value["result"].toString() == "ok")
            {
                m_pageTotal = value["total"].toInt();

                QVariantList comments = value["rows"].toList();
                foreach(const QVariant &comm, comments)
                {
                    if(comm.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    MusicPlaylistItem comment;
                    value = comm.toMap();
                    comment.m_playCount = value["like_num"].toString();
                    comment.m_updateTime = QString::number(QDateTime::fromString(value["time"].toString(),
                                                     "yyyy-MM-dd hh:mm:ss").toMSecsSinceEpoch());
                    comment.m_description = value["msg"].toString();

                    QUrl name;
                    comment.m_nickName = name.fromEncoded(value["u_name"].toByteArray(), QUrl::TolerantMode).toString();
                    comment.m_coverUrl = value["u_pic"].toString();

                    emit createSearchedItems(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}



MusicKWPlaylistCommentsThread::MusicKWPlaylistCommentsThread(QObject *parent)
    : MusicDownLoadCommentsThread(parent)
{
    m_pageSize = 20;
}

QString MusicKWPlaylistCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicKWPlaylistCommentsThread::startToSearch(const QString &name)
{
    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));

    m_rawData["songID"] = name;
    startToPage(0);
}

void MusicKWPlaylistCommentsThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();
    m_pageTotal = 0;
    m_interrupt = true;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(KW_PL_COMMIT_URL, false)
                    .arg(m_rawData["songID"].toString()).arg(offset + 1).arg(m_pageSize);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicKWPlaylistCommentsThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
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
            if(value["result"].toString() == "ok")
            {
                m_pageTotal = value["total"].toInt();

                QVariantList comments = value["rows"].toList();
                foreach(const QVariant &comm, comments)
                {
                    if(comm.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    MusicPlaylistItem comment;
                    value = comm.toMap();
                    comment.m_playCount = value["like_num"].toString();
                    comment.m_updateTime = QString::number(QDateTime::fromString(value["time"].toString(),
                                                     "yyyy-MM-dd hh:mm:ss").toMSecsSinceEpoch());
                    comment.m_description = value["msg"].toString();

                    QUrl name;
                    comment.m_nickName = name.fromEncoded(value["u_name"].toByteArray(), QUrl::TolerantMode).toString();
                    comment.m_coverUrl = value["u_pic"].toString();

                    emit createSearchedItems(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
