#include "musicnetworkabstract.h"

MusicNetworkAbstract::MusicNetworkAbstract(QObject *parent)
    : QObject(parent)
{
    m_reply = nullptr;
    m_manager = nullptr;
}

MusicNetworkAbstract::~MusicNetworkAbstract()
{
    deleteAll();
}

QString MusicNetworkAbstract::getClassName()
{
    return staticMetaObject.className();
}

void MusicNetworkAbstract::deleteAll()
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

void MusicNetworkAbstract::replyError(QNetworkReply::NetworkError)
{
    M_LOGGER_ERROR("Abnormal network connection");
    emit downLoadDataChanged(QString());
    deleteAll();
}

#ifndef QT_NO_SSL
void MusicNetworkAbstract::sslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    sslErrorsString(reply, errors);
    emit downLoadDataChanged(QString());
    deleteAll();
}

void MusicNetworkAbstract::sslErrorsString(QNetworkReply *reply, const QList<QSslError> &errors)
{
    QString errorString;
    foreach(const QSslError &error, errors)
    {
        if(!errorString.isEmpty())
        {
            errorString += ", ";
        }
        errorString += error.errorString();
    }

    M_LOGGER_ERROR(QString("sslErrors: %1").arg(errorString));
    reply->ignoreSslErrors();
}
#endif
