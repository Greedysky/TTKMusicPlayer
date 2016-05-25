#include "musicdownloadquerythreadabstract.h"

MusicDownLoadQueryThreadAbstract::MusicDownLoadQueryThreadAbstract(QObject *parent)
    : QObject(parent), m_reply(nullptr)
{
    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                       SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("MusicDownLoadQueryThreadAbstract Support ssl: %1").arg(QSslSocket::supportsSsl()));
#endif
    m_queryAllRecords = false;
}

MusicDownLoadQueryThreadAbstract::~MusicDownLoadQueryThreadAbstract()
{
    deleteAll();///The release of all the objects
    if(m_manager)
    {
        m_manager->deleteLater();
        m_manager = nullptr;
    }
}

void MusicDownLoadQueryThreadAbstract::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}

void MusicDownLoadQueryThreadAbstract::replyError(QNetworkReply::NetworkError)
{
    M_LOGGER_ERROR("Abnormal network connection");
    emit resolvedSuccess();
    deleteAll();
}

#ifndef QT_NO_SSL
void MusicDownLoadQueryThreadAbstract::sslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
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
    emit resolvedSuccess();
    deleteAll();
}
#endif
