#include "musicbdtranslationrequest.h"

#define TRANSLATION_URL    "TXRkdVhlYnQzSEtZUmpJMVpDeHpaVG5DVzhId0NyVE42YXBPYkw2d25YeGJENDBONm9kSVZ2My95eHgvbVJSQjlDSE92clVkam85OG9uYjU="

MusicBDTranslationRequest::MusicBDTranslationRequest(QObject *parent)
    : MusicTranslationRequest(parent)
{

}

void MusicBDTranslationRequest::startRequest(const QString &data)
{
    startRequest(Language::Auto, Language::Zh, data);
}

void MusicBDTranslationRequest::downLoadFinished()
{
    MusicTranslationRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
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
                break;
            }
        }
        else
        {
            Q_EMIT downLoadDataChanged(QString());
        }
    }
    else
    {
        TTK_ERROR_STREAM("Translation source data error");
        Q_EMIT downLoadDataChanged(QString());
    }

    deleteAll();
}

QString MusicBDTranslationRequest::mapTypeFromEnumToString(Language type) const
{
    switch(type)
    {
        case Language::Auto: return "auto";
        case Language::Ara: return "ara";
        case Language::De: return "de";
        case Language::Ru: return "ru";
        case Language::Fra: return "fra";
        case Language::Kor: return "kor";
        case Language::Nl: return "nl";
        case Language::Pt: return "pt";
        case Language::Jp: return "jp";
        case Language::Th: return "th";
        case Language::Wyw: return "wyw";
        case Language::Spa: return "spa";
        case Language::El: return "el";
        case Language::It: return "it";
        case Language::En: return "en";
        case Language::Yue: return "yue";
        case Language::Zh: return "zh";
        default: return QString();
    }
}

void MusicBDTranslationRequest::startRequest(Language from, Language to, const QString &data)
{
    TTK_INFO_STREAM(QString("%1 startToSearch").arg(className()));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(TRANSLATION_URL, false).arg(mapTypeFromEnumToString(from), data, mapTypeFromEnumToString(to)));
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}
