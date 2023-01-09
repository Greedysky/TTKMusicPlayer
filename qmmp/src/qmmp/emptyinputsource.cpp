#include "emptyinputsource_p.h"

EmptyInputSource::EmptyInputSource(const QString &url, QObject *parent)
    : InputSource(url, parent)
{

}

QIODevice *EmptyInputSource::ioDevice() const
{
    return nullptr;
}

bool EmptyInputSource::initialize()
{
    m_ok = true; //check decoders
    if(m_ok)
        emit ready();
    return m_ok;
}

bool EmptyInputSource::isReady() const
{
    return m_ok;
}
