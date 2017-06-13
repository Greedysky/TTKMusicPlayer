#include "musicdownloadquerybdcommentsthread.h"
#include "musicdownloadquerybdthread.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"
#include "qaeswrap.h"

MusicDownLoadQueryBDCommentsThread::MusicDownLoadQueryBDCommentsThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_pageSize = 20;
    m_queryServer = "Baidu";
}

MusicDownLoadQueryBDCommentsThread::~MusicDownLoadQueryBDCommentsThread()
{
    deleteAll();
}

QString MusicDownLoadQueryBDCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryBDCommentsThread::startToSearch(QueryType type, const QString &name)
{
    Q_UNUSED(type);

    MusicSemaphoreLoop loop;
    MusicDownLoadQueryBDThread *query = new MusicDownLoadQueryBDThread(this);
    query->setQueryAllRecords(false);
    query->setQuerySimplify(true);
    query->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, name);
    connect(query, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    m_rawData["songID"] = 0;
    if(!query->getMusicSongInfos().isEmpty())
    {
        m_rawData["songID"] = query->getMusicSongInfos().first().m_songId.toInt();
        startToSearch(0);
    }
}

void MusicDownLoadQueryBDCommentsThread::startToSearch(int offset)
{
    if(!m_manager)
    {
        return;
    }

    deleteAll();
    m_pageTotal = 0;

    QString time = "1494911685";
    QString key = QCryptographicHash::hash(QString("baidu_taihe_music_secret_key" + time).toUtf8(), QCryptographicHash::Md5).toHex().mid(8, 16);
    QString data = MusicCryptographicHash::decryptData(BD_SG_COMMIT_DATA_URL, URL_KEY).arg(m_pageSize*offset).arg(m_pageSize).arg(m_rawData["songID"].toInt());
    QString eKey = QAesWrap::encrypt(data.toUtf8(), key.toUtf8(), key.toUtf8());
    QString sign = QCryptographicHash::hash(QString("baidu_taihe_music" + eKey + time).toUtf8(), QCryptographicHash::Md5).toHex();
    eKey.replace('+', "%2B");
    eKey.replace('/', "%2F");
    eKey.replace('=', "%3D");
    QUrl musicUrl = MusicCryptographicHash::decryptData(BD_SG_COMMIT_URL, URL_KEY).arg(time).arg(sign).arg(eKey);

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

void MusicDownLoadQueryBDCommentsThread::downLoadFinished()
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
            if(value["error_code"].toInt() == 22000 && value.contains("result"))
            {
                value = value["result"].toMap();
                m_pageTotal = value["commentlist_last_nums"].toLongLong();

                QVariantList comments = value["commentlist_last"].toList();
                foreach(const QVariant &comm, comments)
                {
                    MusicSongCommentItem comment;
                    value = comm.toMap();
                    comment.m_likedCount = QString::number(value["zan_num"].toLongLong());
                    comment.m_time = QString::number(value["ctime"].toLongLong()*1000);
                    comment.m_content = value["comment"].toString();

                    QVariantMap user = value["author"].toMap();
                    comment.m_nickName = user["username"].toString();
                    comment.m_avatarUrl = user["userpic"].toString();

                    emit createSearchedItems(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}
