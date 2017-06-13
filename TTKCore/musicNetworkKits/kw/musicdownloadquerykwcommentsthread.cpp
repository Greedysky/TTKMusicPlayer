#include "musicdownloadquerykwcommentsthread.h"
#include "musicdownloadquerykwthread.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryKWCommentsThread::MusicDownLoadQueryKWCommentsThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_pageSize = 20;
    m_queryServer = "Kuwo";
}

MusicDownLoadQueryKWCommentsThread::~MusicDownLoadQueryKWCommentsThread()
{
    deleteAll();
}

QString MusicDownLoadQueryKWCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryKWCommentsThread::startToSearch(QueryType type, const QString &name)
{
    Q_UNUSED(type);

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
        startToSearch(0);
    }
}

void MusicDownLoadQueryKWCommentsThread::startToSearch(int offset)
{
    if(!m_manager)
    {
        return;
    }

    deleteAll();
    m_pageTotal = 0;

    QUrl musicUrl = MusicCryptographicHash::decryptData(KW_SG_COMMIT_URL, URL_KEY)
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

void MusicDownLoadQueryKWCommentsThread::downLoadFinished()
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
            if(value["result"].toString() == "ok")
            {
                m_pageTotal = value["total"].toString().toInt();

                QVariantList comments = value["rows"].toList();
                foreach(const QVariant &comm, comments)
                {
                    MusicSongCommentItem comment;
                    value = comm.toMap();
                    comment.m_likedCount = value["like_num"].toString();
                    comment.m_time = QString::number(QDateTime::fromString(value["time"].toString(),
                                                     "yyyy-MM-dd hh:mm:ss").toMSecsSinceEpoch());
                    comment.m_content = value["msg"].toString();

                    QUrl name;
                    comment.m_nickName = name.fromEncoded(value["u_name"].toByteArray(), QUrl::TolerantMode).toString();
                    comment.m_avatarUrl = value["u_pic"].toString();

                    emit createSearchedItems(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
