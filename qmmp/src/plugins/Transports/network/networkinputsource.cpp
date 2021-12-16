#include "networkinputsource.h"
#include "networkstreamreader.h"

NetworkInputSource::NetworkInputSource(const QString &url, QObject *parent)
    : InputSource(url, parent)
{
    m_reader = new NetworkStreamReader(url, this);
    connect(m_reader, SIGNAL(ready()), SIGNAL(ready()));
    connect(m_reader, SIGNAL(error()), SIGNAL(error()));
}

QIODevice *NetworkInputSource::ioDevice() const
{
    return m_reader;
}

bool NetworkInputSource::initialize()
{
    m_reader->run();
    return true;
}

bool NetworkInputSource::isReady() const
{
    return m_reader->isOpen();
}

bool NetworkInputSource::isWaiting() const
{
    return !m_reader->bytesAvailable() && m_reader->isOpen();
}

QString NetworkInputSource::contentType() const
{
    return m_reader->contentType();
}
