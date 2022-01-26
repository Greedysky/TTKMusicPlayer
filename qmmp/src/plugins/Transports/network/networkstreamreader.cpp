#include "networkstreamreader.h"
#include "networkinputsource.h"

#include <QFile>
#include <QSettings>
#include <QApplication>
#include <qmmp/statehandler.h>

NetworkStreamReader::NetworkStreamReader(const QString &url, QObject *parent)
    : QIODevice(parent),
      m_url(url)
{
    m_path = m_url.section("#", -1);
    if(m_url == m_path)
    {
        m_path.clear();
    }

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Network");
    m_buffer_size = settings.value("buffer_size", 256).toInt() * 1024;
    if(!m_path.isEmpty())
    {
        m_path = settings.value("buffer_path").toString() + m_path;
    }
    settings.endGroup();
}

NetworkStreamReader::~NetworkStreamReader()
{
    abort();
    m_stream.buffer_size = 0;
    m_stream.buffer_fill = 0;
    m_stream.buffer.clear();
}

bool NetworkStreamReader::atEnd() const
{
    return false;
}

qint64 NetworkStreamReader::bytesAvailable() const
{
    return QIODevice::bytesAvailable() + m_stream.buffer_size;
}

qint64 NetworkStreamReader::bytesToWrite() const
{
    return -1;
}

void NetworkStreamReader::close()
{
    abort();
    QIODevice::close();
}

bool NetworkStreamReader::isSequential() const
{
    return true;
}

bool NetworkStreamReader::open(OpenMode mode)
{
    if(m_ready && mode == QIODevice::ReadOnly)
    {
        QIODevice::open(mode);
        return true;
    }

    return false;
}

bool NetworkStreamReader::seek(qint64 time)
{
    Q_UNUSED(time);
    return false;
}

void NetworkStreamReader::run()
{
    qDebug("NetworkStreamReader: starting download thread");
    QNetworkRequest request;
    request.setUrl(m_url);
    request.setRawHeader("Icy-MetaData", "1");

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(readyRead()), SLOT(handleReadyRead()));
    connect(m_reply, SIGNAL(finished()), SLOT(handleFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

QString NetworkStreamReader::contentType() const
{
    return m_stream.content_type.toLower();
}

void NetworkStreamReader::replyError(QNetworkReply::NetworkError status)
{
    qDebug("NetworkStreamReader: replyError %d", status);
    m_path.clear();
    emit error();
    QIODevice::close();
}

void NetworkStreamReader::handleReadyRead()
{
    const QByteArray &data = m_reply->readAll();
    m_mutex.lock();
    m_stream.buffer.push_back(data);
    m_stream.buffer_size += data.size();
    m_mutex.unlock();

    if(m_stream.aborted || m_ready)
    {
        return;
    }

    if(m_stream.buffer_size > m_buffer_size)
    {
        m_ready  = true;
        NetworkInputSource *object = static_cast<NetworkInputSource*>(parent());

        QHash<QString, QString> info;
        QMap<Qmmp::MetaData, QString> metaData;
        for(const QByteArray &header : m_reply->rawHeaderList())
        {
            if(header.toLower() == "icy-name")
            {
                metaData.insert(Qmmp::TITLE, m_reply->rawHeader("icy-name"));
                info.insert("icy-name", m_reply->rawHeader("icy-name"));
            }
            else if(header.toLower() == "icy-genre")
            {
                metaData.insert(Qmmp::GENRE, m_reply->rawHeader("icy-genre"));
                info.insert("icy-genre", m_reply->rawHeader("icy-genre"));
            }
            else if(header.toLower() == "icy-br")
            {
                object->setProperty(Qmmp::BITRATE, m_reply->rawHeader("icy-br"));
            }
        }

        if(!metaData.isEmpty())
        {
            object->addMetaData(metaData);
            object->addStreamInfo(info);
        }

        m_stream.content_type = m_reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug("NetworkStreamReader: has ready");
        emit ready();
    }
    else
    {
        StateHandler::instance()->dispatchBuffer(100 * m_stream.buffer_size / m_buffer_size);
        qApp->processEvents();
    }
}

void NetworkStreamReader::handleFinished()
{
    if(m_path.isEmpty() || m_stream.buffer_size <= 0 || m_stream.aborted)
    {
        return;
    }

    QFile file(m_path);
    if(file.open(QFile::WriteOnly))
    {
        qDebug("NetworkStreamReader: cache file to %s", QmmpPrintable(m_path));
        file.write(m_stream.buffer);
        file.close();
    }
}

qint64 NetworkStreamReader::readData(char* data, qint64 maxlen)
{
    m_mutex.lock();
    if(m_stream.buffer_size == 0)
    {
        m_mutex.unlock();
        return 0;
    }

    const qint64 len = readBuffer(data, maxlen);
    m_mutex.unlock();
    return len;
}

qint64 NetworkStreamReader::writeData(const char*, qint64)
{
    return -1;
}

void NetworkStreamReader::abort()
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

qint64 NetworkStreamReader::readBuffer(char* data, qint64 maxlen)
{
    if(m_stream.buffer_size > 0 && !m_stream.aborted)
    {
        const int len = qMin<qint64>(m_stream.buffer_size, maxlen);
        if(m_stream.buffer_fill >= m_stream.buffer_size)
        {
            m_stream.buffer_fill = 0;
            return 0;
        }

        memcpy(data, m_stream.buffer.constData() + m_stream.buffer_fill, len);
        m_stream.buffer_fill += len;
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
