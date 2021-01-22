#include "musicmgcommentsrequest.h"
#include "musicmgqueryrequest.h"
#include "musicsemaphoreloop.h"

MusicMGSongCommentsRequest::MusicMGSongCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = 5;
}

void MusicMGSongCommentsRequest::startToSearch(const QString &name)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));

    MusicSemaphoreLoop loop;
    MusicMGQueryRequest *d = new MusicMGQueryRequest(this);
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

void MusicMGSongCommentsRequest::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(MG_COMMENT_SONG_URL, false).arg(m_rawData["songID"].toString()).arg(offset + 1).arg(m_pageSize));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(MG_UA_URL, ALG_UA_KEY, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(MG_REFERER_URL, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicMGSongCommentsRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));

    setNetworkAbort(false);

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("returnCode") && value["returnCode"].toString() == "000000")
            {
                value = value["data"].toMap();
                m_totalSize = value["itemTotal"].toLongLong();

                const QVariantList &datas = value["items"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem comment;
                    comment.m_playCount = value["praiseCount"].toLongLong();
                    comment.m_updateTime = value["createTime"].toString();
                    comment.m_description = value["body"].toString();

                    const QVariantMap &user = value["author"].toMap();
                    comment.m_nickName = user["name"].toString();
                    comment.m_coverUrl = user["avatar"].toString();
                    if(!comment.m_coverUrl.contains(TTK_HTTPM))
                    {
                        comment.m_coverUrl.insert(0, TTK_HTTPM);
                    }
                    Q_EMIT createSearchedItem(comment);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}



MusicMGPlaylistCommentsRequest::MusicMGPlaylistCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = 5;
}

void MusicMGPlaylistCommentsRequest::startToSearch(const QString &name)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));

    m_rawData["songID"] = name;
    startToPage(0);
}

void MusicMGPlaylistCommentsRequest::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(MG_COMMENT_PLAYLIST_URL, false).arg(m_rawData["songID"].toString()).arg(offset + 1).arg(m_pageSize));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(MG_UA_URL, ALG_UA_KEY, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(MG_REFERER_URL, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicMGPlaylistCommentsRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));

    setNetworkAbort(false);

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("returnCode") && value["returnCode"].toString() == "000000")
            {
                value = value["data"].toMap();
                m_totalSize = value["itemTotal"].toLongLong();

                const QVariantList &datas = value["items"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem comment;
                    comment.m_playCount = value["praiseCount"].toLongLong();
                    comment.m_updateTime = value["createTime"].toString();
                    comment.m_description = value["body"].toString();

                    const QVariantMap &user = value["author"].toMap();
                    comment.m_nickName = user["name"].toString();
                    comment.m_coverUrl = user["avatar"].toString();
                    if(!comment.m_coverUrl.contains(TTK_HTTPM))
                    {
                        comment.m_coverUrl.insert(0, TTK_HTTPM);
                    }
                    Q_EMIT createSearchedItem(comment);
                }
            }
        }
    }


    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
