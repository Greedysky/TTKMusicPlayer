#include "musicmgsongsuggestrequest.h"
#include "musicmgqueryinterface.h"

MusicMGSongSuggestRequest::MusicMGSongSuggestRequest(QObject *parent)
    : MusicSongSuggestRequest(parent)
{

}

void MusicMGSongSuggestRequest::startToSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(MG_SUGGEST_URL, false).arg(text));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(MG_UA_URL, ALG_UA_KEY, false).toUtf8());
    request.setRawHeader("Referer", MusicUtils::Algorithm::mdII(MG_REFERER_URL, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicMGSongSuggestRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));

    m_items.clear();
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
                const QVariantList &datas = value["songs"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultsItem item;
                    item.m_name = value["name"].toString();
                    item.m_nickName = value["singerName"].toString();
                    m_items << item;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
