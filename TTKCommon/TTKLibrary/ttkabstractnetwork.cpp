#include "ttkabstractnetwork.h"
#if !TTK_QT_VERSION_CHECK(5,8,0)
#include <QBuffer>
#endif

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

void TTKAbstractNetwork::downloadFinished()
{
    m_interrupt = false;
}

void TTKAbstractNetwork::replyError(QNetworkReply::NetworkError error)
{
    TTK_ERROR_STREAM("Abnormal network connection, module" << this << "code" << error);
    Q_EMIT downloadDataChanged({});
    deleteAll();
}

#ifndef QT_NO_SSL
void TTKAbstractNetwork::sslErrors(QNetworkReply* reply, const QList<QSslError> &errors)
{
    sslErrorsString(reply, errors);
    Q_EMIT downloadDataChanged({});
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

    TTK_ERROR_STREAM("SslErrors:" << errorString);
    reply->ignoreSslErrors();
}
#endif



void TTK::setUserAgentHeader(QNetworkRequest *request, const QByteArray &data) noexcept
{
    request->setRawHeader("User-Agent", data.isEmpty() ? QByteArray("Mozilla/5.0 (X11; Linux x86_64; rv:141.0) Gecko/20100101 Firefox/141.0") : data);
}

void TTK::setContentTypeHeader(QNetworkRequest *request, const QByteArray &data) noexcept
{
    request->setRawHeader("Content-Type", data.isEmpty() ? QByteArray("application/x-www-form-urlencoded") : data);
}

void TTK::setSslConfiguration(QNetworkRequest *request, QSslSocket::PeerVerifyMode mode) noexcept
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

qint64 TTK::fetchFileSizeByUrl(const QString &url)
{
    qint64 size = -1;

    QNetworkRequest request;
    request.setUrl(url);
    TTK::setSslConfiguration(&request);
    TTK::setContentTypeHeader(&request);

    TTKEventLoop loop;
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.head(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QtNetworkErrorVoidConnect(reply, &loop, quit, TTK_SLOT);
    loop.exec();

    if(reply->error() != QNetworkReply::NoError)
    {
        TTK_INFO_STREAM(reply->error() << reply->errorString() << reply->readAll());
        return size;
    }

    size = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    const QVariant &redirection = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(redirection.isValid())
    {
        size = fetchFileSizeByUrl(redirection.toString());
    }

    reply->deleteLater();
    return size;
}

QByteArray TTK::syncNetworkQueryForGet(QNetworkRequest *request)
{
    TTKEventLoop loop;
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(*request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QtNetworkErrorVoidConnect(reply, &loop, quit, TTK_SLOT);
    loop.exec();

    if(reply->error() != QNetworkReply::NoError)
    {
        TTK_INFO_STREAM(reply->error() << reply->errorString() << reply->readAll());
        return {};
    }

    const QVariant &redirection = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(redirection.isValid())
    {
        reply->deleteLater();
        return redirection.toString().toUtf8();
    }

    const QByteArray bytes(reply->readAll());
    reply->deleteLater();
    return bytes;
}

QByteArray TTK::syncNetworkQueryForPost(QNetworkRequest *request, const QByteArray &data)
{
    TTKEventLoop loop;
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.post(*request, data);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QtNetworkErrorVoidConnect(reply, &loop, quit, TTK_SLOT);
    loop.exec();

    if(reply->error() != QNetworkReply::NoError)
    {
        TTK_INFO_STREAM(reply->error() << reply->errorString() << reply->readAll());
        return {};
    }

    const QByteArray bytes(reply->readAll());
    reply->deleteLater();
    return bytes;
}

QByteArray TTK::syncNetworkQueryForPut(QNetworkRequest *request, const QByteArray &data)
{
    TTKEventLoop loop;
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.put(*request, data);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QtNetworkErrorVoidConnect(reply, &loop, quit, TTK_SLOT);
    loop.exec();

    if(reply->error() != QNetworkReply::NoError)
    {
        TTK_INFO_STREAM(reply->error() << reply->errorString() << reply->readAll());
        return {};
    }

    const QByteArray bytes(reply->readAll());
    reply->deleteLater();
    return bytes;
}

QByteArray TTK::syncNetworkQueryForPatch(QNetworkRequest *request, const QByteArray &data)
{
    TTKEventLoop loop;
    QNetworkAccessManager manager;
#if TTK_QT_VERSION_CHECK(5,8,0)
    QNetworkReply *reply = manager.sendCustomRequest(*request, "PATCH", data);
#else
    QBuffer *buffer = new QBuffer;
    buffer->setData(data);
    if(!buffer->open(QIODevice::ReadOnly))
    {
        delete buffer;
        return {};
    }

    QNetworkReply *reply = manager.sendCustomRequest(*request, "PATCH", buffer);
    buffer->setParent(reply);
#endif
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QtNetworkErrorVoidConnect(reply, &loop, quit, TTK_SLOT);
    loop.exec();

    if(reply->error() != QNetworkReply::NoError)
    {
        TTK_INFO_STREAM(reply->error() << reply->errorString() << reply->readAll());
        return {};
    }

    const QByteArray bytes(reply->readAll());
    reply->deleteLater();
    return bytes;
}
