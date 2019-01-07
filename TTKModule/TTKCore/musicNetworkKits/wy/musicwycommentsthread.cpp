#include "musicwycommentsthread.h"
#include "musicdownloadquerywythread.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"

MusicWYSongCommentsThread::MusicWYSongCommentsThread(QObject *parent)
    : MusicDownLoadCommentsThread(parent)
{
    m_pageSize = 20;
}

void MusicWYSongCommentsThread::startToSearch(const QString &name)
{
    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));
    MusicSemaphoreLoop loop;
    MusicDownLoadQueryWYThread *d = new MusicDownLoadQueryWYThread(this);
    d->setQueryAllRecords(false);
    d->setQuerySimplify(true);
    d->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, name);
    connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    m_rawData["songID"] = 0;
    if(!d->isEmpty())
    {
        m_rawData["songID"] = d->getMusicSongInfos().first().m_songId.toInt();
        startToPage(0);
    }
}

void MusicWYSongCommentsThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();

    m_pageTotal = 0;
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(WY_SG_COMMIT_N_URL, false).arg(m_rawData["songID"].toInt()),
                      MusicUtils::Algorithm::mdII(WY_COMMIT_NDT_URL, false).arg(m_rawData["songID"].toInt()).arg(m_pageSize).arg(m_pageSize*offset));
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicWYSongCommentsThread::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toLongLong() == 200)
            {
                m_pageTotal = value["total"].toLongLong();
                const QVariantList &comments = value["comments"].toList();
                foreach(const QVariant &comm, comments)
                {
                    if(comm.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    MusicResultsItem comment;
                    value = comm.toMap();
                    const QVariantMap &user = value["user"].toMap();
                    comment.m_nickName = user["nickname"].toString();
                    comment.m_coverUrl = user["avatarUrl"].toString();

                    comment.m_playCount = QString::number(value["likedCount"].toLongLong());
                    comment.m_updateTime = QString::number(value["time"].toLongLong());
                    comment.m_description = value["content"].toString();

                    emit createSearchedItem(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}



MusicWYPlaylistCommentsThread::MusicWYPlaylistCommentsThread(QObject *parent)
    : MusicDownLoadCommentsThread(parent)
{
    m_pageSize = 20;
}

void MusicWYPlaylistCommentsThread::startToSearch(const QString &name)
{
    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));

    m_rawData["songID"] = name;
    startToPage(0);
}

void MusicWYPlaylistCommentsThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();

    m_pageTotal = 0;
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(WY_PL_COMMIT_N_URL, false).arg(m_rawData["songID"].toLongLong()),
                      MusicUtils::Algorithm::mdII(WY_COMMIT_NDT_URL, false).arg(m_rawData["songID"].toLongLong()).arg(m_pageSize).arg(m_pageSize*offset));
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicWYPlaylistCommentsThread::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toLongLong() == 200)
            {
                m_pageTotal = value["total"].toLongLong();
                const QVariantList &comments = value["comments"].toList();
                foreach(const QVariant &comm, comments)
                {
                    if(comm.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    MusicResultsItem comment;
                    value = comm.toMap();
                    const QVariantMap &user = value["user"].toMap();
                    comment.m_nickName = user["nickname"].toString();
                    comment.m_coverUrl = user["avatarUrl"].toString();

                    comment.m_playCount = QString::number(value["likedCount"].toLongLong());
                    comment.m_updateTime = QString::number(value["time"].toLongLong());
                    comment.m_description = value["content"].toString();

                    emit createSearchedItem(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
