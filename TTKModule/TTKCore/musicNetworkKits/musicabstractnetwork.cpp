#include "musicabstractnetwork.h"

MusicAbstractNetwork::MusicAbstractNetwork(QObject *parent)
    : QObject(parent)
{
    m_interrupt = false;
    m_stateCode = MusicObject::NetworkQuery;
    m_reply = nullptr;
#ifndef QT_NO_SSL
    connect(&m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
#endif
}

MusicAbstractNetwork::~MusicAbstractNetwork()
{
    m_interrupt = true;
    m_stateCode = MusicObject::NetworkError;

    deleteAll();
}

void MusicAbstractNetwork::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
    m_interrupt = true;
}

void MusicAbstractNetwork::downLoadFinished()
{
    m_interrupt = false;
}

void MusicAbstractNetwork::replyError(QNetworkReply::NetworkError)
{
    TTK_LOGGER_ERROR("Abnormal network connection");
    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

#ifndef QT_NO_SSL
void MusicAbstractNetwork::sslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    sslErrorsString(reply, errors);
    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicAbstractNetwork::sslErrorsString(QNetworkReply *reply, const QList<QSslError> &errors)
{
    QString errorString;
    for(const QSslError &error : qAsConst(errors))
    {
        if(!errorString.isEmpty())
        {
            errorString += ", ";
        }
        errorString += error.errorString();
    }

    TTK_LOGGER_ERROR(QString("sslErrors: %1").arg(errorString));
    reply->ignoreSslErrors();
}
#endif

namespace MusicObject
{
void setSslConfiguration(QNetworkRequest *request, QSslSocket::PeerVerifyMode mode)
{
    request->setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request->sslConfiguration();
    sslConfig.setPeerVerifyMode(mode);
    request->setSslConfiguration(sslConfig);
#else
    Q_UNUSED(request);
    Q_UNUSED(mode);
#endif
}

QByteArray syncNetworkQueryForGet(QNetworkRequest *request)
{
    MusicSemaphoreLoop loop;
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(*request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    QObject::connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
#else
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
#endif
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return QByteArray();
    }

    const QByteArray bytes(reply->readAll());
    reply->deleteLater();
    return bytes;
}

QByteArray syncNetworkQueryForPost(QNetworkRequest *request, const QByteArray &data)
{
    MusicSemaphoreLoop loop;
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.post(*request, data);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    QObject::connect(reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
#else
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
#endif
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return QByteArray();
    }

    const QByteArray bytes(reply->readAll());
    reply->deleteLater();
    return bytes;
}
}
