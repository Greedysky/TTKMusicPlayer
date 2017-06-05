#include "musicdownloadquerykgcommentsthread.h"
#include "musicdownloadquerykgthread.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryKGCommentsThread::MusicDownLoadQueryKGCommentsThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_pageSize = 20;
    m_queryServer = "Kugou";
}

MusicDownLoadQueryKGCommentsThread::~MusicDownLoadQueryKGCommentsThread()
{
    deleteAll();
}

QString MusicDownLoadQueryKGCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryKGCommentsThread::startSearchSong(QueryType type, const QString &name)
{
    Q_UNUSED(type);

    MusicSemaphoreLoop loop;
    MusicDownLoadQueryKGThread *query = new MusicDownLoadQueryKGThread(this);
    query->setQueryAllRecords(false);
    query->setQuerySimplify(true);
    query->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, name);
    connect(query, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    m_rawData["songID"].clear();
    if(!query->getMusicSongInfos().isEmpty())
    {
        m_rawData["songID"] = query->getMusicSongInfos().first().m_songId;
        startSearchSong(0);
    }
}

void MusicDownLoadQueryKGCommentsThread::startSearchSong(int offset)
{
    if(!m_manager)
    {
        return;
    }

    deleteAll();
    m_pageTotal = 0;

    QUrl musicUrl = MusicCryptographicHash::decryptData(KG_SG_COMMIT_URL, URL_KEY)
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

void MusicDownLoadQueryKGCommentsThread::downLoadFinished()
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
