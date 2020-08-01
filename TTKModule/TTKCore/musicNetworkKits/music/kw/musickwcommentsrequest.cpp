#include "musickwcommentsrequest.h"
#include "musickwqueryrequest.h"
#include "musicsemaphoreloop.h"

MusicKWSongCommentsRequest::MusicKWSongCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = 20;
}

void MusicKWSongCommentsRequest::startToSearch(const QString &name)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));
    MusicSemaphoreLoop loop;
    MusicKWQueryRequest *d = new MusicKWQueryRequest(this);
    d->setQueryAllRecords(false);
    d->setQuerySimplify(true);
    d->startToSearch(MusicAbstractQueryRequest::MusicQuery, name);
    connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    m_rawData["songID"].clear();
    if(!d->isEmpty())
    {
        m_rawData["songID"] = d->getMusicSongInfos().first().m_songId;
        startToPage(0);
    }
}

void MusicKWSongCommentsRequest::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(KW_COMMENT_SONG_URL, false).arg(m_rawData["songID"].toString()).arg(offset + 1).arg(m_pageSize);
    m_pageTotal = 0;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicKWSongCommentsRequest::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
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
            if(value["result"].toString() == "ok")
            {
                m_pageTotal = value["total"].toInt();

                const QVariantList &comments = value["rows"].toList();
                foreach(const QVariant &comm, comments)
                {
                    if(comm.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    MusicResultsItem comment;
                    value = comm.toMap();

                    comment.m_playCount = value["like_num"].toString();
                    comment.m_updateTime = QString::number(QDateTime::fromString(value["time"].toString(), MUSIC_YEAR_STIME_FORMAT).toMSecsSinceEpoch());
                    comment.m_description = value["msg"].toString();
                    comment.m_nickName = QUrl::fromEncoded(value["u_name"].toByteArray(), QUrl::TolerantMode).toString();
                    comment.m_coverUrl = value["u_pic"].toString();

                    Q_EMIT createSearchedItem(comment);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}



MusicKWPlaylistCommentsRequest::MusicKWPlaylistCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = 20;
}

void MusicKWPlaylistCommentsRequest::startToSearch(const QString &name)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));

    m_rawData["songID"] = name;
    startToPage(0);
}

void MusicKWPlaylistCommentsRequest::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(KW_COMMENT_PLAYLIST_URL, false).arg(m_rawData["songID"].toString()).arg(offset + 1).arg(m_pageSize);
    m_pageTotal = 0;
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KW_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicKWPlaylistCommentsRequest::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
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
            if(value["result"].toString() == "ok")
            {
                m_pageTotal = value["total"].toInt();

                const QVariantList &comments = value["rows"].toList();
                foreach(const QVariant &comm, comments)
                {
                    if(comm.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    MusicResultsItem comment;
                    value = comm.toMap();

                    comment.m_playCount = value["like_num"].toString();
                    comment.m_updateTime = QString::number(QDateTime::fromString(value["time"].toString(), MUSIC_YEAR_STIME_FORMAT).toMSecsSinceEpoch());
                    comment.m_description = value["msg"].toString();
                    comment.m_nickName = QUrl().fromEncoded(value["u_name"].toByteArray(), QUrl::TolerantMode).toString();
                    comment.m_coverUrl = value["u_pic"].toString();

                    Q_EMIT createSearchedItem(comment);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
