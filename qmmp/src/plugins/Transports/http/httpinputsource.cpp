#include "httpinputsource.h"
#include "httpstreamreader.h"

HttpInputSource::HttpInputSource(const QString &url, QObject *parent)
    : InputSource(url, parent)
{
    m_reader = new HttpStreamReader(url, this);
    connect(m_reader, SIGNAL(ready()), SIGNAL(ready()));
    connect(m_reader, SIGNAL(error()), SIGNAL(error()));
}

QIODevice *HttpInputSource::ioDevice() const
{
    return m_reader;
}

bool HttpInputSource::initialize()
{
    m_reader->run();
    return true;
}

bool HttpInputSource::isReady() const
{
    return m_reader->isOpen();
}

bool HttpInputSource::isWaiting() const
{
    return !m_reader->bytesAvailable() && m_reader->isOpen();
}

QString HttpInputSource::contentType() const
{
    return m_reader->contentType();
}
