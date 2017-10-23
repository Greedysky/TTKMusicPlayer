#include "qnputextra.h"

#include <QMap>

class QNPutExtraPrivate : public MusicPrivate<QNPutExtra>
{
public:
    QNPutExtraPrivate();

    void insert(const QString &key, const QString &value);
    QString remove(const QString &key);

    QMap<QString, QString> m_params;
    QString m_mimeType;
    qint32 m_crc32, m_checkCrc32;

//    static const int NO_CRC32 = 0;
//    static const int AUTO_CRC32 = 1;
//    static const int WITH_CRC32 = 2;

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

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
///
QNPutExtra::QNPutExtra()
{
    MUSIC_INIT_PRIVATE;
}

void QNPutExtra::addExtraParam(const QString &key, const QString &value)
{
    MUSIC_D(QNPutExtra);
    d->insert(key, value);
}

QString QNPutExtra::removeExtraParam(const QString &key)
{
    MUSIC_D(QNPutExtra);
    return d->remove(key);
}

QString QNPutExtra::getMimeType() const
{
    MUSIC_D(QNPutExtra);
    return d->m_mimeType;
}

void QNPutExtra::setMimeType(const QString &value)
{
    MUSIC_D(QNPutExtra);
    d->m_mimeType = value;
}

qint32 QNPutExtra::getCrc32() const
{
    MUSIC_D(QNPutExtra);
    return d->m_crc32;
}

void QNPutExtra::setCrc32(qint32 value)
{
    MUSIC_D(QNPutExtra);
    d->m_crc32 = value;
}

qint32 QNPutExtra::getCheckCrc32() const
{
    MUSIC_D(QNPutExtra);
    return d->m_checkCrc32;
}

void QNPutExtra::setCheckCrc32(qint32 value)
{
    MUSIC_D(QNPutExtra);
    d->m_checkCrc32 = value;
}
