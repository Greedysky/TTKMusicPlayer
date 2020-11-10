#include "musicabstractnetwork.h"

MusicAbstractNetwork::MusicAbstractNetwork(QObject *parent)
    : QObject(parent)
{
    m_interrupt = false;
    m_stateCode = MusicObject::NetworkQuery;
    m_reply = nullptr;
    m_manager = nullptr;
}

MusicAbstractNetwork::~MusicAbstractNetwork()
{
    m_stateCode = MusicObject::NetworkSuccess;
    deleteAll();
}

void MusicAbstractNetwork::deleteAll()
{
    if(m_manager)
    {
        m_manager->deleteLater();
        m_manager = nullptr;
    }
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
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
