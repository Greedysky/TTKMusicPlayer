#include "qnputextra.h"

#include <QMap>

class QNPutExtraPrivate : public TTKPrivate<QNPutExtra>
{
public:
    QNPutExtraPrivate();

    void insert(const QString &key, const QString &value);
    QString remove(const QString &key);

    MStringMap m_params;
    QString m_mimeType;
    qint32 m_crc32, m_checkCrc32;
};

QNPutExtraPrivate::QNPutExtraPrivate()
{
    m_crc32 = -1;
    m_checkCrc32 = -1;
}

void QNPutExtraPrivate::insert(const QString &key, const QString &value)
{
    m_params.insert(key, value);
}

QString QNPutExtraPrivate::remove(const QString &key)
{
    QString value;
    if(m_params.contains(key))
    {
        value = m_params.value(key);
        m_params.remove(key);
    }
    return value;
}



QNPutExtra::QNPutExtra()
{
    TTK_INIT_PRIVATE;
}

void QNPutExtra::addExtraParam(const QString &key, const QString &value)
{
    TTK_D(QNPutExtra);
    d->insert(key, value);
}

QString QNPutExtra::removeExtraParam(const QString &key)
{
    TTK_D(QNPutExtra);
    return d->remove(key);
}

QString QNPutExtra::getMimeType() const
{
    TTK_D(QNPutExtra);
    return d->m_mimeType;
}

void QNPutExtra::setMimeType(const QString &value)
{
    TTK_D(QNPutExtra);
    d->m_mimeType = value;
}

qint32 QNPutExtra::getCrc32() const
{
    TTK_D(QNPutExtra);
    return d->m_crc32;
}

void QNPutExtra::setCrc32(qint32 value)
{
    TTK_D(QNPutExtra);
    d->m_crc32 = value;
}

qint32 QNPutExtra::getCheckCrc32() const
{
    TTK_D(QNPutExtra);
    return d->m_checkCrc32;
}

void QNPutExtra::setCheckCrc32(qint32 value)
{
    TTK_D(QNPutExtra);
    d->m_checkCrc32 = value;
}
