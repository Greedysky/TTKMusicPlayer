#include "musicqqcommentsthread.h"
#include "musicdownloadqueryqqthread.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"

MusicQQCommentsThread::MusicQQCommentsThread(QObject *parent)
    : MusicDownLoadCommentsThread(parent)
{
    m_pageSize = 20;
}

QString MusicQQCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicQQCommentsThread::startToSearch(const QString &name)
{
    MusicSemaphoreLoop loop;
    MusicDownLoadQueryQQThread *query = new MusicDownLoadQueryQQThread(this);
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

void MusicQQCommentsThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    deleteAll();
    m_pageTotal = 0;

    QUrl musicUrl = QString(MusicUtils::Algorithm::mdII(QQ_SG_COMMIT_DATA_URL, false));

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->post( request, MusicUtils::Algorithm::mdII(QQ_SG_COMMIT_URL, false)
                               .arg(m_rawData["songID"].toString()).arg(offset).arg(m_pageSize).toUtf8());
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()) );
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     SLOT(replyError(QNetworkReply::NetworkError)) );
}

void MusicQQCommentsThread::downLoadFinished()
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
            if(value["code"].toInt() == 0)
            {
                m_pageTotal = value["commenttotal"].toInt();

                QVariantList comments = value["comment"].toList();
                foreach(const QVariant &comm, comments)
                {
                    MusicSongCommentItem comment;
                    value = comm.toMap();
                    comment.m_likedCount = QString::number(value["praisenum"].toInt());
                    comment.m_time = QString::number(value["time"].toLongLong()*1000);
                    comment.m_content = value["rootcommentcontent"].toString();

                    comment.m_nickName = value["nick"].toString();
                    comment.m_avatarUrl = value["avatarurl"].toString();

                    emit createSearchedItems(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
