#include "musicqqcommentsrequest.h"
#include "musicqqqueryrequest.h"

MusicQQSongCommentsRequest::MusicQQSongCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = 20;
}

void MusicQQSongCommentsRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    MusicSemaphoreLoop loop;
    MusicQQQueryRequest *d = new MusicQQQueryRequest(this);
    d->setQueryAllRecords(false);
    d->setQueryLite(true);
    d->startToSearch(MusicAbstractQueryRequest::MusicQuery, value);
    connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    m_rawData["sid"].clear();
    if(!d->isEmpty())
    {
        m_rawData["sid"] = d->header("sid");
        startToPage(0);
    }
}

void MusicQQSongCommentsRequest::startToPage(int offset)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_BASE_URL, false));
    MusicQQInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.post(request, MusicUtils::Algorithm::mdII(QQ_COMMENT_SONG_URL, false).arg(m_rawData["sid"].toString()).arg(m_pageSize).arg(offset).toUtf8());
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicQQSongCommentsRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicCommentsRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 0)
            {
                QVariantMap req = value["req_1"].toMap();
                req = req["data"].toMap();
                QVariantList datas = req["response_list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    req = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    m_totalSize = req["count"].toInt();
                }

                req = value["req_2"].toMap();
                req = req["data"].toMap();
                QVariantMap comment = req["CommentList2"].toMap();
                datas = comment["Comments"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    req = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem item;
                    item.m_playCount = QString::number(req["PraiseNum"].toInt());
                    item.m_updateTime = QString::number(req["PubTime"].toLongLong());
                    item.m_description = req["Content"].toString();
                    item.m_nickName = req["Nick"].toString();
                    item.m_coverUrl = req["Avatar"].toString();
                    Q_EMIT createSearchedItem(item);
                }

                req = value["req_2"].toMap();
                req = req["data"].toMap();
                comment = req["CommentList"].toMap();
                datas = comment["Comments"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    req = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem item;
                    item.m_playCount = QString::number(req["PraiseNum"].toInt());
                    item.m_updateTime = QString::number(req["PubTime"].toLongLong());
                    item.m_description = req["Content"].toString();
                    item.m_nickName = req["Nick"].toString();
                    item.m_coverUrl = req["Avatar"].toString();
                    Q_EMIT createSearchedItem(item);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}



MusicQQPlaylistCommentsRequest::MusicQQPlaylistCommentsRequest(QObject *parent)
    : MusicCommentsRequest(parent)
{
    m_pageSize = 20;
}

void MusicQQPlaylistCommentsRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    m_rawData["sid"] = value;
    startToPage(0);
}

void MusicQQPlaylistCommentsRequest::startToPage(int offset)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = 0;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_BASE_URL, false));
    MusicQQInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.post(request, MusicUtils::Algorithm::mdII(QQ_COMMENT_PLAYLIST_URL, false).arg(m_rawData["sid"].toString()).arg(m_pageSize).arg(offset).toUtf8());
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicQQPlaylistCommentsRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicCommentsRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 0)
            {
                QVariantMap req = value["req_1"].toMap();
                req = req["data"].toMap();
                QVariantList datas = req["response_list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    req = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    m_totalSize = req["count"].toInt();
                }

                req = value["req_2"].toMap();
                req = req["data"].toMap();
                QVariantMap comment = req["CommentList2"].toMap();
                datas = comment["Comments"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    req = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem item;
                    item.m_playCount = QString::number(req["PraiseNum"].toInt());
                    item.m_updateTime = QString::number(req["PubTime"].toLongLong());
                    item.m_description = req["Content"].toString();
                    item.m_nickName = req["Nick"].toString();
                    item.m_coverUrl = req["Avatar"].toString();
                    Q_EMIT createSearchedItem(item);
                }

                req = value["req_2"].toMap();
                req = req["data"].toMap();
                comment = req["CommentList"].toMap();
                datas = comment["Comments"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    req = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem item;
                    item.m_playCount = QString::number(req["PraiseNum"].toInt());
                    item.m_updateTime = QString::number(req["PubTime"].toLongLong());
                    item.m_description = req["Content"].toString();
                    item.m_nickName = req["Nick"].toString();
                    item.m_coverUrl = req["Avatar"].toString();
                    Q_EMIT createSearchedItem(item);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
