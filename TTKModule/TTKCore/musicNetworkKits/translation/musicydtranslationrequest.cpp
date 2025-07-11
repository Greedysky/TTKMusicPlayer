#include "musicydtranslationrequest.h"

static constexpr const char *QUERY_URL = "alNydEx6cDhBTklzNFBuUmJiOVk0RVZJZEZOUXRvVHVCcWxxeW4vTnFmbU1QTjBHT2ZuWUxxQU1SMlVPMklJeERSS2xJZTl2Sm1KWVNtUGxCb2JneGUrVTZxQXY2cjhmUmdHNk81QkYrdjVUYVJwZlFPa0NlQ05melF4MXJLUXMrWk5vcmtCWTRFb0pBSE9aeVhUR3FvcnI4SnV0REVwaWRKZk1QZz09";
static constexpr const char *TRANSLATION_URL = "MERYdllhU0NxcW1KS1BNQjFwYnQ2UXBDQmlkSUJ3S05tNUdSSUJxcnZjZlBpY0hRWGVVTkh0TjhFOFhlMnlDOExpY2VnVmlmWE1XMlhrK2l2YXl0T3RVbk5CdWo1R0I5UFFla1NBZ0tUK3hvaVA4SUsyNFRoRVlIQXJSbU5xaHlsUGU1N0svb1NNNjVvUWRLV2VvR2NKY2g4Zms9";

MusicYDTranslationRequest::MusicYDTranslationRequest(QObject *parent)
    : MusicAbstractTranslationRequest(parent)
{

}

void MusicYDTranslationRequest::startToRequest(const QString &data)
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    MusicAbstractNetwork::deleteAll();

    QString sid;
    {
        QNetworkRequest request;
        request.setUrl(TTK::Algorithm::mdII(QUERY_URL, false));
        TTK::setSslConfiguration(&request);
        TTK::makeContentTypeHeader(&request);

        const QString &bytes = QString(TTK::syncNetworkQueryForGet(&request));
        if(!bytes.isEmpty())
        {
            const QRegExp regx("sid\\:\\s\\'([0-9a-f\\.]+)");
            sid = (regx.indexIn(bytes) != -1) ? regx.cap(1) : bytes;
        }
    }

    if(sid.isEmpty())
    {
        TTK_INFO_STREAM(className() << "downLoadFinished");
        Q_EMIT downLoadDataChanged({});
        deleteAll();
        return;
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(TRANSLATION_URL, false).arg(sid, mapToString(Language::Chinese), data));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicYDTranslationRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    MusicAbstractTranslationRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value["code"].toInt() == 200)
            {
                const QVariantList &datas = value["text"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    Q_EMIT downLoadDataChanged(var.toString());
                    deleteAll();
                    return;
                }
            }
        }
    }

    TTK_ERROR_STREAM("Translation source data error");
    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

QString MusicYDTranslationRequest::mapToString(Language type) const noexcept
{
    switch(type)
    {
        case Language::Auto: return "auto";
        case Language::Chinese: return "zh";
        default: return {};
    }
}
