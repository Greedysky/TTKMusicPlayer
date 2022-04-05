#include "musicbdtranslationrequest.h"
#include "musicbdqueryinterface.h"

MusicBDTranslationRequest::MusicBDTranslationRequest(QObject *parent)
    : MusicTranslationRequest(parent)
{

}

void MusicBDTranslationRequest::startToDownload(const QString &data)
{
    startToDownload(MusicBDTranslationRequest::Type_Auto, MusicBDTranslationRequest::Type_Zh, data);
}

void MusicBDTranslationRequest::startToDownload(TranslationType from, TranslationType to, const QString &data)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch").arg(className()));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(BD_TRANSLATION_URL, false).arg(mapTypeFromEnumToString(from), data, mapTypeFromEnumToString(to)));
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

QString MusicBDTranslationRequest::mapTypeFromEnumToString(TranslationType type) const
{
    switch(type)
    {
        case Type_Auto: return "auto";
        case Type_Ara: return "ara";
        case Type_De: return "de";
        case Type_Ru: return "ru";
        case Type_Fra: return "fra";
        case Type_Kor: return "kor";
        case Type_Nl: return "nl";
        case Type_Pt: return "pt";
        case Type_Jp: return "jp";
        case Type_Th: return "th";
        case Type_Wyw: return "wyw";
        case Type_Spa: return "spa";
        case Type_El: return "el";
        case Type_It: return "it";
        case Type_En: return "en";
        case Type_Yue: return "yue";
        case Type_Zh: return "zh";
        default: return QString();
    }
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
        TTK_LOGGER_ERROR("Translation source data error");
        Q_EMIT downLoadDataChanged(QString());
    }

    deleteAll();
}
