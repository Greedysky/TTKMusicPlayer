#include "musictranslationthread.h"
#include "musicobject.h"
#///QJson import
#include "qjson/parser.h"

const QString TRANSLATION_URL = "TXRkdVhlYnQzSEtZUmpJMVpDeHpaVG5DVzhId0NyVE42YXBPYkw2d25YeGJENDBONm9kSVZ2My95eHgvbVJSQjlDSE92clVkam85OG9uYjU=";

MusicTranslationThread::MusicTranslationThread(QObject *parent)
    : MusicTranslationThreadAbstract(parent)
{

}

MusicTranslationThread::~MusicTranslationThread()
{
    deleteAll();
}

void MusicTranslationThread::startToDownload(const QString &data)
{
    startToDownload(MusicTranslationThread::Type_Auto, MusicTranslationThread::Type_Zh, data);
}

void MusicTranslationThread::startToDownload(TranslationType from, TranslationType to, const QString &data)
{
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(TRANSLATION_URL, false).arg(mapTypeFromEnumToString(from)).arg(data).arg(mapTypeFromEnumToString(to)));
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

QString MusicTranslationThread::mapTypeFromEnumToString(TranslationType type)
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

void MusicTranslationThread::downLoadFinished()
{
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            value = value["trans_result"].toMap();
            const QVariantList &datas = value["data"].toList();
            foreach(const QVariant &var, datas)
            {
                value = var.toMap();
                if(value.isEmpty() || value["dst"].toString().isEmpty())
                {
                    continue;
                }
                emit downLoadDataChanged(value["dst"].toString());
                break;
            }
        }
        else
        {
            emit downLoadDataChanged(QString());
        }
    }
    else
    {
        M_LOGGER_ERROR("Translation source data error");
        emit downLoadDataChanged(QString());
    }
    deleteAll();
}
