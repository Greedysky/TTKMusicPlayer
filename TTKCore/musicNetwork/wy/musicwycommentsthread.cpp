#include "musicwycommentsthread.h"
#include "musicdownloadquerywythread.h"
#include "musicdownloadquerywythread.h"

#///QJson import
#include "qjson/parser.h"


MusicWYCommentsThread::MusicWYCommentsThread(QObject *parent)
    : MusicNetworkAbstract(parent)
{
    m_count = 0;
    m_songID = 0;

    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                       SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
#endif
}

MusicWYCommentsThread::~MusicWYCommentsThread()
{
    deleteAll();
}

QString MusicWYCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicWYCommentsThread::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}

void MusicWYCommentsThread::startSearchSong(const QString &name)
{
    QEventLoop loop;
    MusicDownLoadQueryWYThread *query = new MusicDownLoadQueryWYThread(this);
    query->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, name);
    connect(query, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    m_songID = 0;
    if(!query->getMusicSongInfos().isEmpty())
    {
        m_songID = query->getMusicSongInfos().first().m_songId.toInt();
    }
}

void MusicWYCommentsThread::startSearchSong(int index)
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    m_count = 0;
    QUrl musicUrl = MusicCryptographicHash::decryptData(WY_SONG_COMMIT_URL, URL_KEY).arg(m_songID).arg(COMMIT_PAGE_SIZE*index);

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
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()) );
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     SLOT(replyError(QNetworkReply::NetworkError)) );
}

void MusicWYCommentsThread::downLoadFinished()
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
            if(value["code"].toLongLong() == 200)
            {
                m_count = value["total"].toLongLong();

                QVariantList hots = value["comments"].toList();
                foreach(const QVariant &hot, hots)
                {
                    MusicSongComment comment;
                    value = hot.toMap();
                    QVariantMap user = value["user"].toMap();
                    comment.m_nickName = user["nickname"].toString();
                    comment.m_avatarUrl = user["avatarUrl"].toString();

                    comment.m_likedCount = QString::number(value["likedCount"].toLongLong());
                    comment.m_time = QString::number(value["time"].toLongLong());
                    comment.m_content = value["content"].toString();

                    emit createSearchedItems(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
