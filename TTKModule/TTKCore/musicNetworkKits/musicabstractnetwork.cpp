#include "musicabstractnetwork.h"

MusicAbstractNetwork::MusicAbstractNetwork(QObject *parent)
    : QObject(parent)
{
    m_interrupt = false;
    m_stateCode = MusicObject::NetworkQuery;
    m_reply = nullptr;
    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
#endif
}

MusicAbstractNetwork::~MusicAbstractNetwork()
{
    m_interrupt = true;
    m_stateCode = MusicObject::NetworkError;

    deleteAll();
    if(m_manager)
    {
        m_manager->deleteLater();
        m_manager = nullptr;
    }
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
}
