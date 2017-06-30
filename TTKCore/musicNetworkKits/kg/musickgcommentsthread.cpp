#include "musickgcommentsthread.h"
#include "musicdownloadquerykgthread.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"

MusicKGCommentsThread::MusicKGCommentsThread(QObject *parent)
    : MusicDownLoadCommentsThread(parent)
{
    m_pageSize = 20;
}

QString MusicKGCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicKGCommentsThread::startToSearch(const QString &name)
{
    MusicSemaphoreLoop loop;
    MusicDownLoadQueryKGThread *query = new MusicDownLoadQueryKGThread(this);
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

void MusicKGCommentsThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    deleteAll();
    m_pageTotal = 0;

    QUrl musicUrl = MusicUtils::Algorithm::mdII(KG_SG_COMMIT_URL, false)
                    .arg(m_rawData["songID"].toString()).arg(offset + 1).arg(m_pageSize);
    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
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

void MusicKGCommentsThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["err_code"].toInt() == 0)
            {
                m_pageTotal = value["count"].toLongLong();

                QVariantList comments = value["list"].toList();
                foreach(const QVariant &comm, comments)
                {
                    MusicSongCommentItem comment;
                    value = comm.toMap();

                    comment.m_likedCount = QString::number(value["like"].toMap()["count"].toLongLong());
                    comment.m_time = QString::number(QDateTime::fromString(value["addtime"].toString(),
                                                     "yyyy-MM-dd hh:mm:ss").toMSecsSinceEpoch());
                    comment.m_content = value["content"].toString();

                    comment.m_nickName = value["user_name"].toString();
                    comment.m_avatarUrl = value["user_pic"].toString();

                    emit createSearchedItems(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
