#include "musictranslationthread.h"
#include "musicobject.h"
#///QJson import
#include "qjson/parser.h"

MusicTranslationThread::MusicTranslationThread(QObject *parent)
    : MusicNetworkAbstract(parent)
{
    m_reply = nullptr;
    m_manager = nullptr;
}

MusicTranslationThread::~MusicTranslationThread()
{
    deleteAll();
}

QString MusicTranslationThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicTranslationThread::startToTranslation(TranslationType from, TranslationType to, const QString &data)
{
    m_manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl( MusicCryptographicHash::decryptData(TRANSLATION_URL, URL_KEY).arg(mapTypeFromEnumToString(from))
                                            .arg(data).arg(mapTypeFromEnumToString(to)) );
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                       SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));

    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif

    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
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
        QByteArray bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            value = value["trans_result"].toMap();
            QVariantList datas = value["data"].toList();
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
