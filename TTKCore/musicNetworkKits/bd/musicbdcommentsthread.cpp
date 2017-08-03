#include "musicbdcommentsthread.h"
#include "musicdownloadquerybdthread.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"
#include "qaeswrap.h"

MusicBDCommentsThread::MusicBDCommentsThread(QObject *parent)
    : MusicDownLoadCommentsThread(parent)
{
    m_pageSize = 20;
}

QString MusicBDCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicBDCommentsThread::startToSearch(const QString &name)
{
    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));

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
        startToPage(0);
    }
}

void MusicBDCommentsThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToPage %2").arg(getClassName()).arg(offset));
    deleteAll();
    m_pageTotal = 0;

    QString time = "1494911685";
    QString key = MusicUtils::Algorithm::md5(QString("baidu_taihe_music_secret_key" + time).toUtf8()).toHex().mid(8, 16);
    QString data = MusicUtils::Algorithm::mdII(BD_SG_COMMIT_DATA_URL, false).arg(m_pageSize*offset).arg(m_pageSize).arg(m_rawData["songID"].toInt());
    QString eKey = QAesWrap::encrypt(data.toUtf8(), key.toUtf8(), key.toUtf8());
    QString sign = MusicUtils::Algorithm::md5(QString("baidu_taihe_music" + eKey + time).toUtf8()).toHex();
    eKey.replace('+', "%2B");
    eKey.replace('/', "%2F");
    eKey.replace('=', "%3D");
    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_SG_COMMIT_URL, false).arg(time).arg(sign).arg(eKey);

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
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)) );
}

void MusicBDCommentsThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
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
    M_LOGGER_INFO(QString("%1 finished deleteAll").arg(getClassName()));
}
