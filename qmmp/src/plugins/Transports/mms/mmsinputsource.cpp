#include "mmsinputsource.h"
#include "mmsstreamreader.h"

MMSInputSource::MMSInputSource(const QString &url, QObject *parent)
    : InputSource(url, parent)
{
    m_reader = new MMSStreamReader(url, this);
    connect(m_reader, SIGNAL(ready()), SIGNAL(ready()));
    connect(m_reader, SIGNAL(error()), SIGNAL(error()));
}

QIODevice *MMSInputSource::ioDevice() const
{
    return m_reader;
}

bool MMSInputSource::initialize()
{
    m_reader->run();
    return true;
}

bool MMSInputSource::isReady() const
{
    return m_reader->isOpen();
}

bool MMSInputSource::isWaiting() const
{
    return !m_reader->bytesAvailable() && m_reader->isOpen();
}

QString MMSInputSource::contentType() const
{
    return "audio/x-ms-wma";
}
