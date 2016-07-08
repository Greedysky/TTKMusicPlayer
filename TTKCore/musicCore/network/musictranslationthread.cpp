#include "musictranslationthread.h"
#include "musicobject.h"

#ifdef MUSIC_GREATER_NEW
#   include <QJsonArray>
#   include <QJsonObject>
#   include <QJsonValue>
#   include <QJsonParseError>
#else
#   include <QtScript/QScriptEngine>
#   include <QtScript/QScriptValue>
#   include <QtScript/QScriptValueIterator>
#endif

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
    request.setUrl( TRANSLATION_URL.arg(mapTypeFromEnumToString(from))
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
#ifdef MUSIC_GREATER_NEW
        QJsonParseError jsonError;
        QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
        ///Put the data into Json
        if(jsonError.error != QJsonParseError::NoError ||
           !parseDoucment.isObject())
        {
            deleteAll();
            emit downLoadDataChanged(QString());
            return ;
        }

        QJsonObject jsonObject = parseDoucment.object();
        if(jsonObject.contains("error"))
        {
            emit downLoadDataChanged(QString());
            deleteAll();
            return ;
        }

        if(jsonObject.contains("trans_result"))
        {
            jsonObject = jsonObject.value("trans_result").toObject();
            if(jsonObject.contains("data"))
            {
                QJsonArray array = jsonObject.value("data").toArray();
                foreach(QJsonValue value, array)
                {
                    if(!value.isObject())
                    {
                       continue;
                    }
                    QJsonObject obj = value.toObject();
                    emit downLoadDataChanged(obj.value("dst").toString());
                    break;
                }
            }
        }
#else
        QScriptEngine engine;
        QScriptValue sc = engine.evaluate("value=" + QString(bytes));

        if(!sc.property("trans_result").isNull())
        {
            sc = sc.property("trans_result");
            if(sc.property("data").isArray())
            {
                QScriptValueIterator it(sc.property("data"));
                while(it.hasNext())
                {
                    it.next();
                    QScriptValue value = it.value();
                    if(value.isNull() || value.property("dst").toString().isEmpty())
                    {
                        continue;
                    }
                    emit downLoadDataChanged(value.property("dst").toString());
                    break;
                }
            }
        }
#endif
    }
    else
    {
        M_LOGGER_ERROR("Translation source data error");
        emit downLoadDataChanged(QString());
    }
    deleteAll();
}
