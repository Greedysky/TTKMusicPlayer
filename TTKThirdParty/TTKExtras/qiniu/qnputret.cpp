#include "qnputret.h"
#///QJson import
#include "qjson/parser.h"

class QNPutRetPrivate : public TTKPrivate<QNPutRet>
{
public:
    QNPutRetPrivate();

    QString m_hash, m_key, m_error;

};

QNPutRetPrivate::QNPutRetPrivate()
{

}



QNPutRet::QNPutRet()
{
    TTK_INIT_PRIVATE;
}

QNPutRet* QNPutRet::fromJSON(const QByteArray &jsonData)
{
    QNPutRet *putRet = new QNPutRet;
    QJson::Parser parser;
    bool ok;
    const QVariant &data = parser.parse(jsonData, &ok);
    if(ok)
    {
        const QVariantMap &value = data.toMap();
        putRet->setHash(value["hash"].toString());
        putRet->setKey(value["key"].toString());
    }
    return putRet;
}

QString QNPutRet::getHash() const
{
    TTK_D(QNPutRet);
    return d->m_hash;
}

void QNPutRet::setHash(const QString &value)
{
    TTK_D(QNPutRet);
    d->m_hash = value;
}

QString QNPutRet::getKey() const
{
    TTK_D(QNPutRet);
    return d->m_key;
}

void QNPutRet::setKey(const QString &value)
{
    TTK_D(QNPutRet);
    d->m_key = value;
}
