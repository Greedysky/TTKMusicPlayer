#include "mmsstreamreader.h"
#include "mmsinputsource.h"

#include <QSettings>
#include <QApplication>
#include <qmmp/statehandler.h>

MMSStreamReader::MMSStreamReader(const QString &url, QObject *parent)
    : QIODevice(parent),
      m_url(url)
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("MMS");
    m_bufferSize = settings.value("buffer_size", 256).toInt() * 1024;
    settings.endGroup();
}

MMSStreamReader::~MMSStreamReader()
{
    abort();
    m_stream.size = 0;
    m_stream.fill = 0;
    m_stream.buffer.clear();
}

bool MMSStreamReader::atEnd() const
{
    return false;
}

qint64 MMSStreamReader::bytesAvailable() const
{
    return QIODevice::bytesAvailable() + m_stream.size;
}

qint64 MMSStreamReader::bytesToWrite() const
{
    return -1;
}

void MMSStreamReader::close()
{
    abort();
    QIODevice::close();
}

bool MMSStreamReader::isSequential() const
{
    return true;
}

bool MMSStreamReader::open(OpenMode mode)
{
    if(m_ready && mode == QIODevice::ReadOnly)
    {
        QIODevice::open(mode);
        return true;
    }

    return false;
}

bool MMSStreamReader::seek(qint64 time)
{
    Q_UNUSED(time);
    return false;
}

void MMSStreamReader::run()
{
    qDebug("MMSStreamReader: starting download thread");
    m_process = new QProcess(this);
    connect(m_process, SIGNAL(readyRead()), SLOT(handleReadyRead()));
#if QT_VERSION < QT_VERSION_CHECK(5,6,0)
    connect(m_process, SIGNAL(error(QProcess::ProcessError)), SLOT(handleError()));
#else
    connect(m_process, SIGNAL(errorOccurred(QProcess::ProcessError)), SLOT(handleError()));
#endif
    QStringList arguments;
    arguments << "-softvol" << "-slave" << "-quiet" << "-vo" << "directx:noaccel" << "-ao" << "null" << m_url;
    m_process->start(Qmmp::ttkPluginPath() + "/avplayer.tkx", arguments);
}

void MMSStreamReader::handleError()
{
    emit error();
    QIODevice::close();
}

void MMSStreamReader::handleReadyRead()
{
    const QByteArray &buffer = m_process->readAll();
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
        qDebug("MMSStreamReader: has ready");
        emit ready();
    }
    else
    {
        StateHandler::instance()->dispatchBuffer(100 * m_stream.size / m_bufferSize);
        qApp->processEvents();
    }
}

qint64 MMSStreamReader::readData(char *data, qint64 maxlen)
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

qint64 MMSStreamReader::writeData(const char *, qint64)
{
    return -1;
}

void MMSStreamReader::abort()
{
    m_mutex.lock();
    m_ready = false;

    if(m_process)
    {
        m_process->kill();
        delete m_process;
        m_process = nullptr;
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

qint64 MMSStreamReader::readBuffer(char* data, qint64 maxlen)
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
