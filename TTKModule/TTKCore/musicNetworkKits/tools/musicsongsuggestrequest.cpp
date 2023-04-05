#include "musicsongsuggestrequest.h"
#include "musicwyqueryinterface.h"

MusicSongSuggestRequest::MusicSongSuggestRequest(QObject *parent)
    : MusicPageQueryRequest(parent)
{

}

void MusicSongSuggestRequest::startToSearch(const QString &value)
{
    TTK_INFO_STREAM(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();

    QNetworkRequest request;
    const QByteArray &parameter = MusicWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_SUGGEST_URL, false),
                      TTK::Algorithm::mdII(WY_SUGGEST_DATA_URL, false).arg(value));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicSongSuggestRequest::downLoadFinished()
    {
        TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

        m_items.clear();
        MusicSongSuggestRequest::downLoadFinished();

        if(m_reply && m_reply->error() == QNetworkReply::NoError)
        {
            QJson::Parser json;
            bool ok = false;
            const QVariant &data = json.parse(m_reply->readAll(), &ok);
            if(ok)
            {
                QVariantMap value = data.toMap();
                if(value["code"].toInt() == 200 && value.contains("result"))
                {
                    value = value["result"].toMap();

                    const QVariantList &datas = value["songs"].toList();
                    for(const QVariant &var : qAsConst(datas))
                    {
                        if(var.isNull())
                        {
                            continue;
                        }

                        value = var.toMap();
                        TTK_NETWORK_QUERY_CHECK();

                        MusicResultDataItem result;
                        result.m_name = TTK::String::charactersReplace(value["name"].toString());
                        const QVariantList &artistsArray = value["artists"].toList();
                        for(const QVariant &artistValue : qAsConst(artistsArray))
                        {
                            if(artistValue.isNull())
                            {
                                continue;
                            }

                            const QVariantMap &artistObject = artistValue.toMap();
                            result.m_nickName = TTK::String::charactersReplace(artistObject["name"].toString());
                        }
                        m_items << result;
                    }
                }
            }
        }

        Q_EMIT downLoadDataChanged(QString());
        deleteAll();
    }
