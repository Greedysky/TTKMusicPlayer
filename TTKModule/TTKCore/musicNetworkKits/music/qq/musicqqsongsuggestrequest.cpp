#include "musicqqsongsuggestrequest.h"
#include "musicqqqueryinterface.h"

MusicQQSongSuggestRequest::MusicQQSongSuggestRequest(QObject *parent)
    : MusicSongSuggestRequest(parent)
{

}

void MusicQQSongSuggestRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QQ_SUGGEST_URL, false).arg(value));
    MusicQQInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicQQSongSuggestRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    m_items.clear();
    MusicSongSuggestRequest::downLoadFinished();

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 0 && value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["song"].toMap();
                const QVariantList &datas = value["itemlist"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem item;
                    item.m_name = MusicUtils::String::charactersReplaced(value["name"].toString());
                    item.m_nickName = MusicUtils::String::charactersReplaced(value["singer"].toString());
                    m_items << item;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
