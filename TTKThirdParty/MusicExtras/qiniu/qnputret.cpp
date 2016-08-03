#include "qnputret.h"

#ifdef MUSIC_GREATER_NEW
#   include <QJsonDocument>
#   include <QJsonObject>
#else
#   ///QJson import
#   include "qjson/parser.h"
#endif

class QNPutRetPrivate : public TTKPrivate<QNPutRet>
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
    TTK_INIT_PRIVATE;
}

QNPutRet* QNPutRet::fromJSON(const QByteArray &jsonData)
{
    QNPutRet *putRet = new QNPutRet;
#ifdef MUSIC_GREATER_NEW
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);//check error
    QJsonObject json = doc.object();
    putRet->setHash(json["hash"].toString());
    putRet->setKey(json["key"].toString());
#else
    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(jsonData, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        putRet->setHash(value["hash"].toString());
        putRet->setKey(value["key"].toString());
    }
#endif
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
