#include "httpstreamreader.h"
#include "httpinputsource.h"

#include <QFile>
#include <QSettings>
#include <QApplication>
#include <QSslConfiguration>
#include <qmmp/statehandler.h>

HttpStreamReader::HttpStreamReader(const QString &url, QObject *parent)
    : QIODevice(parent),
      m_url(url)
{
    m_path = m_url.section("#", -1);
    if(m_url == m_path)
    {
        m_path.clear();
    }

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Http");
    m_bufferSize = settings.value("buffer_size", 256).toInt() * 1024;
    if(!m_path.isEmpty())
    {
        m_path = settings.value("buffer_path").toString() + m_path;
    }
    settings.endGroup();
}

HttpStreamReader::~HttpStreamReader()
{
    abort();
    m_stream.size = 0;
    m_stream.fill = 0;
    m_stream.buffer.clear();
}

bool HttpStreamReader::atEnd() const
{
    return false;
}

qint64 HttpStreamReader::bytesAvailable() const
{
    return QIODevice::bytesAvailable() + m_stream.size;
}

qint64 HttpStreamReader::bytesToWrite() const
{
    return -1;
}

void HttpStreamReader::close()
{
    abort();
    QIODevice::close();
}

bool HttpStreamReader::isSequential() const
{
    return true;
}

bool HttpStreamReader::open(OpenMode mode)
{
    if(m_ready && mode == QIODevice::ReadOnly)
    {
        QIODevice::open(mode);
        return true;
    }

    return false;
}

bool HttpStreamReader::seek(qint64 time)
{
    Q_UNUSED(time);
    return false;
}

void HttpStreamReader::run()
{
    qDebug("HttpStreamReader: starting download thread");
    QNetworkRequest request;
    request.setUrl(m_url);
    request.setRawHeader("Icy-MetaData", "1");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(readyRead()), SLOT(handleReadyRead()));
    connect(m_reply, SIGNAL(finished()), SLOT(handleFinished()));
#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(handleError()));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(handleError()));
#endif
}

QString HttpStreamReader::contentType() const
{
    return m_stream.type.toLower();
}

void HttpStreamReader::handleReadyRead()
{
    const QByteArray &buffer = m_reply->readAll();
    m_mutex.lock();
    m_stream.buffer.append(buffer);
    m_stream.size += buffer.length();
    m_mutex.unlock();

    if(m_stream.aborted || m_ready)
    {
        return;
    }

    if(m_stream.size > m_bufferSize)
    {
        m_ready  = true;
        HttpInputSource *object = static_cast<HttpInputSource*>(parent());

        QHash<QString, QString> info;
        QMap<Qmmp::MetaData, QString> metaData;
        for(const QByteArray &header : m_reply->rawHeaderList())
        {
            const QByteArray &lower = header.toLower();
            if(lower == "icy-name")
            {
                metaData.insert(Qmmp::TITLE, m_reply->rawHeader("icy-name"));
                info.insert("icy-name", m_reply->rawHeader("icy-name"));
            }
            else if(lower == "icy-genre")
            {
                metaData.insert(Qmmp::GENRE, m_reply->rawHeader("icy-genre"));
                info.insert("icy-genre", m_reply->rawHeader("icy-genre"));
            }
            else if(lower == "icy-br")
            {
                object->setProperty(Qmmp::BITRATE, m_reply->rawHeader("icy-br"));
            }
        }

        if(!metaData.isEmpty())
        {
            object->addMetaData(metaData);
            object->addStreamInfo(info);
        }

        m_stream.type = m_reply->header(QNetworkRequest::ContentTypeHeader).toString();
        if(m_stream.type.isEmpty())
        {
            m_stream.type = "application/octet-stream‌";
        }

        qDebug("HttpStreamReader: has ready");
        emit ready();
    }
    else
    {
        StateHandler::instance()->dispatchBuffer(100 * m_stream.size / m_bufferSize);
        qApp->processEvents();
    }
}

void HttpStreamReader::handleFinished()
{
    const QVariant &redirection = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(redirection.isValid())
    {
        m_url = redirection.toString();
        m_reply->deleteLater();
        run();
        return;
    }

    if(m_path.isEmpty() || m_stream.size <= 0 || m_stream.aborted)
    {
        return;
    }

    QFile file(m_path);
    if(file.open(QIODevice::WriteOnly))
    {
        qDebug("HttpStreamReader: cache file to %s", QmmpPrintable(m_path));
        file.write(m_stream.buffer);
        file.close();
    }
}

void HttpStreamReader::handleError(QNetworkReply::NetworkError status)
{
    qDebug("HttpStreamReader: replyError %d", status);
    m_path.clear();
    emit error();
    QIODevice::close();
}

qint64 HttpStreamReader::readData(char *data, qint64 maxlen)
{
    m_mutex.lock();
    if(m_stream.size == 0)
    {
        m_mutex.unlock();
        return 0;
    }

    const qint64 len = readBuffer(data, maxlen);
    m_mutex.unlock();
    return len;
}

qint64 HttpStreamReader::writeData(const char *, qint64)
{
    return -1;
}

void HttpStreamReader::abort()
{
    m_mutex.lock();
    m_ready = false;

    if(m_reply)
    {
        m_reply->abort();
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    if(m_stream.aborted)
    {
        m_mutex.unlock();
        return;
    }

    m_stream.aborted = true;
    m_mutex.unlock();
    QIODevice::close();
}

qint64 HttpStreamReader::readBuffer(char* data, qint64 maxlen)
{
    if(m_stream.size > 0 && !m_stream.aborted)
    {
        const int len = qMin<qint64>(m_stream.size, maxlen);
        if(m_stream.fill >= m_stream.size)
        {
            m_stream.fill = 0;
            m_stream.aborted = true;
            return 0;
        }

        memcpy(data, m_stream.buffer.constData() + m_stream.fill, len);
        m_stream.fill += len;
        return len;
    }
    else if(m_stream.aborted)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
