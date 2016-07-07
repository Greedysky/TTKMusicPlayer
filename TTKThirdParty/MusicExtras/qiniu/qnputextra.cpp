#include "qnputextra.h"

#include <QMap>

QNPutExtra::QNPutExtra()
{
    m_crc32 = -1;
    m_checkCrc32 = -1;
}

QNPutExtra::~QNPutExtra()
{

}

void QNPutExtra::addExtraParam(const QString &key, const QString &value)
{
    m_params.insert(key, value);
}

QString QNPutExtra::removeExtraParam(const QString &key)
{
    QString value;
    if(m_params.contains(key))
    {
        value = m_params.value(key);
        m_params.remove(key);
    }
    return value;
}

QString QNPutExtra::getMimeType() const
{
    return m_mimeType;
}

void QNPutExtra::setMimeType(const QString &value)
{
    m_mimeType = value;
}

qint32 QNPutExtra::getCrc32() const
{
    return m_crc32;
}

void QNPutExtra::setCrc32(qint32 value)
{
    m_crc32 = value;
}

qint32 QNPutExtra::getCheckCrc32() const
{
    return m_checkCrc32;
}

void QNPutExtra::setCheckCrc32(qint32 value)
{
    m_checkCrc32 = value;
}
