#include "musicwscommentsthread.h"
#include "musicsemaphoreloop.h"
#include "musicdownloadquerywsthread.h"
#///QJson import
#include "qjson/parser.h"

#include <QTimer>

MusicWSSongCommentsThread::MusicWSSongCommentsThread(const QString &type, QObject *parent)
    : MusicDownLoadCommentsThread(parent)
{
    m_pageSize = 20;
    m_querySearchType = type;
}

QString MusicWSSongCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicWSSongCommentsThread::startToSearch(const QString &name)
{
    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));
    MusicSemaphoreLoop loop;
    MusicDownLoadQueryWSThread *query = new MusicDownLoadQueryWSThread(m_querySearchType, this);
    query->setQueryAllRecords(false);
    query->setQuerySimplify(true);
    query->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, name);
    connect(query, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    m_rawData["songID"] = 0;
    if(!query->getMusicSongInfos().isEmpty())
    {
        m_rawData["songID"] = query->getMusicSongInfos().first().m_songId.toInt();
        startToPage(0);
    }
}

void MusicWSSongCommentsThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();
    m_pageTotal = m_pageSize;
    m_interrupt = true;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WS_SG_COMMIT_URL, false)
                    .arg(m_rawData["songID"].toInt()).arg(m_querySearchType).arg(m_pageSize);

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

void MusicWSSongCommentsThread::downLoadFinished()
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
            if(value["code"].toLongLong() == 0)
            {
                QVariantList datas = value["data"].toList();
                foreach(const QVariant &data, datas)
                {
                    if(data.isNull())
                    {
                        continue;
                    }

                    value = data.toMap();
                    foreach(const QVariant &comm, value["comments"].toList())
                    {
                        if(comm.isNull())
                        {
                            continue;
                        }

                        if(m_interrupt) return;

                        MusicSongCommentItem comment;
                        value = comm.toMap();
                        QVariantMap user = value["user"].toMap();
                        comment.m_nickName = user["NN"].toString();
                        comment.m_avatarUrl = user["I"].toString();

                        comment.m_likedCount = value["like"].toString();
                        comment.m_time = value["createTime"].toString();
                        comment.m_content = value["content"].toString();

                        emit createSearchedItems(comment);
                    }
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}



MusicWSPlaylistCommentsThread::MusicWSPlaylistCommentsThread(QObject *parent)
    : MusicDownLoadCommentsThread(parent)
{
    m_pageSize = 20;
}

QString MusicWSPlaylistCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicWSPlaylistCommentsThread::startToSearch(const QString &name)
{
    Q_UNUSED(name);
    startToPage(0);
}

void MusicWSPlaylistCommentsThread::startToPage(int offset)
{
    Q_UNUSED(offset);
    QTimer::singleShot(1, this, SLOT(downLoadFinished()));
}

void MusicWSPlaylistCommentsThread::downLoadFinished()
{
    emit downLoadDataChanged(QString());
    deleteAll();
}
