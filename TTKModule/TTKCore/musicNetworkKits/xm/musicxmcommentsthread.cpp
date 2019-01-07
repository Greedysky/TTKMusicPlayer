#include "musicxmcommentsthread.h"
#include "musicdownloadqueryxmthread.h"
#include "musicsemaphoreloop.h"

#///QJson import
#include "qjson/parser.h"

MusicXMSongCommentsThread::MusicXMSongCommentsThread(QObject *parent)
    : MusicDownLoadCommentsThread(parent)
{
    m_pageSize = 20;
}

void MusicXMSongCommentsThread::startToSearch(const QString &name)
{
    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));
    MusicSemaphoreLoop loop;
    MusicDownLoadQueryXMThread *d = new MusicDownLoadQueryXMThread(this);
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

void MusicXMSongCommentsThread::startToPage(int offset)
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
    makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(XM_SG_COMMIT_DATA_URL, false).arg(m_rawData["songID"].toInt()).arg(offset + 1).arg(m_pageSize),
                      MusicUtils::Algorithm::mdII(XM_COMMIT_URL, false));
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicXMSongCommentsThread::downLoadFinished()
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
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["data"].toMap();

                const QVariantMap &paging = value["pagingVO"].toMap();
                m_pageTotal = paging["count"].toLongLong();

                const QVariantList &comments = value["commentVOList"].toList();
                foreach(const QVariant &comm, comments)
                {
                    if(comm.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    MusicResultsItem comment;
                    value = comm.toMap();
                    comment.m_nickName = value["nickName"].toString();
                    comment.m_coverUrl = value["avatar"].toString();

                    if(comment.m_coverUrl.contains("https://"))
                    {
                        comment.m_coverUrl.replace("https://", "http://");
                    }

                    comment.m_playCount = QString::number(value["likes"].toLongLong());
                    comment.m_updateTime = QString::number(value["gmtCreate"].toLongLong());
                    comment.m_description = value["message"].toString();

                    emit createSearchedItem(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}



MusicXMPlaylistCommentsThread::MusicXMPlaylistCommentsThread(QObject *parent)
    : MusicDownLoadCommentsThread(parent)
{
    m_pageSize = 20;
}

void MusicXMPlaylistCommentsThread::startToSearch(const QString &name)
{
    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));

    m_rawData["songID"] = name;
    startToPage(0);
}

void MusicXMPlaylistCommentsThread::startToPage(int offset)
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
    makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(XM_PL_COMMIT_DATA_URL, false).arg(m_rawData["songID"].toInt()).arg(offset + 1).arg(m_pageSize),
                      MusicUtils::Algorithm::mdII(XM_COMMIT_URL, false));
    if(!m_manager || m_stateCode != MusicObject::NetworkInit) return;
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicXMPlaylistCommentsThread::downLoadFinished()
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
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["data"].toMap();

                const QVariantMap &paging = value["pagingVO"].toMap();
                m_pageTotal = paging["count"].toLongLong();

                const QVariantList &comments = value["commentVOList"].toList();
                foreach(const QVariant &comm, comments)
                {
                    if(comm.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    MusicResultsItem comment;
                    value = comm.toMap();
                    comment.m_nickName = value["nickName"].toString();
                    comment.m_coverUrl = value["avatar"].toString();

                    if(comment.m_coverUrl.contains("https://"))
                    {
                        comment.m_coverUrl.replace("https://", "http://");
                    }

                    comment.m_playCount = QString::number(value["likes"].toLongLong());
                    comment.m_updateTime = QString::number(value["gmtCreate"].toLongLong());
                    comment.m_description = value["message"].toString();

                    emit createSearchedItem(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
