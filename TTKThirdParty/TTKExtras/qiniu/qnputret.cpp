#include "qnputret.h"
#///QJson import
#include "qjson/parser.h"

class QNPutRetPrivate : public MusicPrivate<QNPutRet>
{
public:
    QNPutRetPrivate();

    QString m_hash, m_key, m_error;

};

QNPutRetPrivate::QNPutRetPrivate()
{

}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
///
QNPutRet::QNPutRet()
{
    MUSIC_INIT_PRIVATE;
}

QNPutRet* QNPutRet::fromJSON(const QByteArray &jsonData)
{
    QNPutRet *putRet = new QNPutRet;
    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(jsonData, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        putRet->setHash(value["hash"].toString());
        putRet->setKey(value["key"].toString());
    }
    return putRet;
}

QString QNPutRet::getHash() const
{
    MUSIC_D(QNPutRet);
    return d->m_hash;
}

void QNPutRet::setHash(const QString &value)
{
    MUSIC_D(QNPutRet);
    d->m_hash = value;
}

QString QNPutRet::getKey() const
{
    MUSIC_D(QNPutRet);
    return d->m_key;
}

void QNPutRet::setKey(const QString &value)
{
    MUSIC_D(QNPutRet);
    d->m_key = value;
}
