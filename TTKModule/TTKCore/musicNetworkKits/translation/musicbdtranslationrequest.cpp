#include "musicbdtranslationrequest.h"

static constexpr const char *TRANSLATION_URL = "a1ZQMG5kY2dUenpHSGhQUXV6cldhK1A1bkl2c2ROdFJLMTVteTJxeFUzSnlSQXpmRFc0MS9JdlhSMUF0SE1HR2ZYZWcxOXZDalU0SC8rVG8=";

MusicBDTranslationRequest::MusicBDTranslationRequest(QObject *parent)
    : MusicAbstractTranslationRequest(parent)
{

}

void MusicBDTranslationRequest::startToRequest(const QString &data)
{
    TTK_INFO_STREAM(className() << "startToRequest");

    MusicAbstractNetwork::deleteAll();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(TRANSLATION_URL, false).arg(mapToString(Language::Auto), mapToString(Language::Chinese), data));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicBDTranslationRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicAbstractTranslationRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            value = value["trans_result"].toMap();

            const QVariantList &datas = value["data"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                TTK_NETWORK_QUERY_CHECK();

                if(value.isEmpty() || value["dst"].toString().isEmpty())
                {
                    continue;
                }

                Q_EMIT downLoadDataChanged(value["dst"].toString());
                deleteAll();
                return;
            }
        }
    }

    TTK_ERROR_STREAM("Translation source data error");
    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

QString MusicBDTranslationRequest::mapToString(Language type) const
{
    switch(type)
    {
        case Language::Auto: return "auto";
        case Language::Chinese: return "zh";
        default: return {};
    }
}
