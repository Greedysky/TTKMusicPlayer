#include "musicbingtranslationrequest.h"
#include "musicurlutils.h"

static constexpr const char *QUERY_URL = "SUVwbklrTjVFYXdsSUJ3MHRZb21KVDRXL1ZwU1RtSVRNWURYdDZwczMxZnVXenhr";
static constexpr const char *TRANSLATION_URL = "Rm9rVXJIeFNOTDlrV2l5Yld0UGsweGd1V0JZbTFBMVZvQmpKaGhLUmtIT1VhOGtzM0RhVCthdnRhUVNMUGRLYU5LWkJLbVRWQjZoQ2QzYW1wUVIxWkxibnN6VDJ4Q05RNWV6V0NnPT0=";
static constexpr const char *PARAM_URL = "cmFWcURkeGhWb2thdWJIMG5JS3BGQmVseW9XeFU5b0Era3V6V1JMa2IxRmRqY2lFRnpUbkM3TDlsUFVjbUNWdA==";
static constexpr const char *UA_URL = "TWlxK2t4SCthVk1rMjBSWGdOMUdPWVVzSmJSeUtubmsxN1dualdvay8wL0tEVFdETG1BUTJmRlZObUtNM29wVXI1WTNaN1QxSVJNRHFMM1hhclVscEpxbjJpT1FvdnRRZjNtNlZvcnNTcmovQkE3c0s4cVNzUT09";

MusicBingTranslationRequest::MusicBingTranslationRequest(QObject *parent)
    : MusicAbstractTranslationRequest(parent)
{

}

void MusicBingTranslationRequest::startToRequest(const QString &data)
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractNetwork::deleteAll();

    QString ig, token, key;
    {
        QNetworkRequest request;
        request.setUrl(TTK::Algorithm::mdII(QUERY_URL, false));
        TTK::setSslConfiguration(&request);
        TTK::makeContentTypeHeader(&request);

        const QString &bytes = QString(TTK::syncNetworkQueryForGet(&request));
        if(!bytes.isEmpty())
        {
            {
                const QRegExp regx("\"ig\"\\s?:\\s?\"(\\w+)\"");
                ig = (regx.indexIn(bytes) != -1) ? regx.cap(1) : bytes;
            }

            {
                QRegExp regx("params_AbusePreventionHelper\\s?=\\s?\\[(.+)\\]");
                regx.setMinimal(true);
                const QString &buffer = ((regx.indexIn(bytes) != -1) ? regx.cap(1) : QString());

                QJsonParseError ok;
                const QJsonDocument &json = QJsonDocument::fromJson("[" + buffer.toUtf8() + "]", &ok);
                if(QJsonParseError::NoError == ok.error)
                {
                    const QVariantList &datas = json.toVariant().toList();
                    if(datas.count() > 2)
                    {
                        key = datas[0].toString();
                        token = datas[1].toString();
                    }
                }
            }
        }
    }

    if(ig.isEmpty() || key.isEmpty() || token.isEmpty())
    {
        TTK_INFO_STREAM(metaObject()->className() << "downLoadFinished");
        Q_EMIT downLoadDataChanged({});
        deleteAll();
        return;
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(TRANSLATION_URL, false).arg(ig));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);
    TTK::makeUserAgentHeader(&request, TTK::Algorithm::mdII(UA_URL, false).toUtf8());

    auto v = TTK::Algorithm::mdII(PARAM_URL, false).arg(key, token, mapToString(Language::Auto), mapToString(Language::Chinese), data).toUtf8();
    m_reply = m_manager.post(request, TTK::Url::urlPrettyEncode(v));
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicBingTranslationRequest::downLoadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractTranslationRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            for(const QVariant &var : json.toVariant().toList())
            {
                if(var.isNull())
                {
                    continue;
                }

                QVariantMap value = var.toMap();
                for(const QVariant &var : value["translations"].toList())
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    Q_EMIT downLoadDataChanged(value["text"].toString());
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

QString MusicBingTranslationRequest::mapToString(Language type) const noexcept
{
    switch(type)
    {
        case Language::Auto: return "auto-detect";
        case Language::Chinese: return "zh-Hans";
        default: return {};
    }
}
