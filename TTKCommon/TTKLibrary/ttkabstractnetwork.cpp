#include "ttkabstractnetwork.h"

TTKAbstractNetwork::TTKAbstractNetwork(QObject *parent)
    : QObject(parent),
      m_interrupt(false),
      m_stateCode(TTK::NetworkCode::Query),
      m_reply(nullptr)
{
#ifndef QT_NO_SSL
    connect(&m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
#endif
}

TTKAbstractNetwork::~TTKAbstractNetwork()
{
    m_interrupt = true;
    m_stateCode = TTK::NetworkCode::Error;

    deleteAll();
}

void TTKAbstractNetwork::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
    m_interrupt = true;
}

void TTKAbstractNetwork::downLoadFinished()
{
    m_interrupt = false;
}

void TTKAbstractNetwork::replyError(QNetworkReply::NetworkError)
{
    TTK_ERROR_STREAM("Abnormal network connection");
    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

#ifndef QT_NO_SSL
void TTKAbstractNetwork::sslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    sslErrorsString(reply, errors);
    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void TTKAbstractNetwork::sslErrorsString(QNetworkReply *reply, const QList<QSslError> &errors)
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

    TTK_ERROR_STREAM(QString("SslErrors: %1").arg(errorString));
    reply->ignoreSslErrors();
}
#endif

void TTK::makeContentTypeHeader(QNetworkRequest *request, const QByteArray &data)
{
    request->setRawHeader("Content-Type", data.isEmpty() ? "application/x-www-form-urlencoded" : data);
}

void TTK::makeUserAgentHeader(QNetworkRequest *request, const QByteArray &data)
{
    request->setRawHeader("User-Agent", data.isEmpty() ? "Mozilla/5.0 (X11; Linux x86_64; rv:98.0) Gecko/20100101 Firefox/98.0" : data);
}

void TTK::setSslConfiguration(QNetworkRequest *request, QSslSocket::PeerVerifyMode mode)
{
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request->sslConfiguration();
    sslConfig.setPeerVerifyMode(mode);
    request->setSslConfiguration(sslConfig);
#else
    Q_UNUSED(request);
    Q_UNUSED(mode);
#endif
}

qint64 TTK::queryFileSizeByUrl(const QString &url)
{
    qint64 size = -1;

    QNetworkRequest request;
    request.setUrl(url);
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    TTKSemaphoreLoop loop;
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.head(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QtNetworkErrorVoidConnect(reply, &loop, quit);
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return size;
    }

    size = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    const QVariant &redirection = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(redirection.isValid())
    {
        size = queryFileSizeByUrl(redirection.toString());
    }

    reply->deleteLater();
    return size;
}

QByteArray TTK::syncNetworkQueryForGet(QNetworkRequest *request)
{
    TTKSemaphoreLoop loop;
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(*request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QtNetworkErrorVoidConnect(reply, &loop, quit);
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return {};
    }

    const QByteArray bytes(reply->readAll());
    reply->deleteLater();
    return bytes;
}

QByteArray TTK::syncNetworkQueryForPost(QNetworkRequest *request, const QByteArray &data)
{
    TTKSemaphoreLoop loop;
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.post(*request, data);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QtNetworkErrorVoidConnect(reply, &loop, quit);
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return {};
    }

    const QByteArray bytes(reply->readAll());
    reply->deleteLater();
    return bytes;
}
