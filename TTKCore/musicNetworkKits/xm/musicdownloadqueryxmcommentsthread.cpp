#include "musicdownloadqueryxmcommentsthread.h"
#include "musicdownloadqueryxmthread.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"

MusicDownLoadQueryXMCommentsThread::MusicDownLoadQueryXMCommentsThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_pageSize = 20;
    m_queryServer = "XiaMi";
}

MusicDownLoadQueryXMCommentsThread::~MusicDownLoadQueryXMCommentsThread()
{
    deleteAll();
}

QString MusicDownLoadQueryXMCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryXMCommentsThread::startSearchSong(QueryType type, const QString &name)
{
    Q_UNUSED(type);

    MusicSemaphoreLoop loop;
    MusicDownLoadQueryXMThread *query = new MusicDownLoadQueryXMThread(this);
    query->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, name);
    connect(query, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    m_rawData["songID"] = 0;
    if(!query->getMusicSongInfos().isEmpty())
    {
        m_rawData["songID"] = query->getMusicSongInfos().first().m_songId.toInt();
        startSearchSong(0);
    }
}

void MusicDownLoadQueryXMCommentsThread::startSearchSong(int offset)
{
    deleteAll();

    m_pageTotal = 0;
    QNetworkRequest request;
    makeTokenQueryUrl(m_manager, &request,
                      MusicCryptographicHash::decryptData(XM_SG_COMMIT_DATA_URL, URL_KEY).arg(m_rawData["songID"].toInt())
                      .arg(offset + 1).arg(m_pageSize),
                      MusicCryptographicHash::decryptData(XM_SG_COMMIT_URL, URL_KEY));
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

void MusicDownLoadQueryXMCommentsThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request
        bytes = bytes.replace("xiami(", "");
        bytes = bytes.replace("callback(", "");
        bytes.chop(1);

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["data"].toMap();

                QVariantMap paging = value["pagingVO"].toMap();
                m_pageTotal = paging["count"].toLongLong();

                QVariantList comments = value["commentVOList"].toList();
                foreach(const QVariant &comm, comments)
                {
                    MusicSongCommentItem comment;
                    value = comm.toMap();
                    comment.m_nickName = value["nickName"].toString();
                    comment.m_avatarUrl = value["avatar"].toString();

                    comment.m_likedCount = QString::number(value["likes"].toLongLong());
                    comment.m_time = QString::number(value["gmtCreate"].toLongLong());
                    comment.m_content = value["message"].toString();

                    emit createSearchedItems(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
