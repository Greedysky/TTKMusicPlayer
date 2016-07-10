#include "qnputret.h"

#include <QJsonDocument>
#include <QJsonObject>

QNPutRet::QNPutRet()
{

}

QNPutRet::~QNPutRet()
{

}

QNPutRet* QNPutRet::fromJSON(const QByteArray &jsonData)
{
    QNPutRet *putRet = new QNPutRet;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);//check error
    QJsonObject json = doc.object();
    putRet->setHash(json["hash"].toString());
    putRet->setKey(json["key"].toString());
    return putRet;
}

QString QNPutRet::getHash() const
{
    return m_hash;
}

void QNPutRet::setHash(const QString &value)
{
    m_hash = value;
}

QString QNPutRet::getKey() const
{
    return m_key;
}

void QNPutRet::setKey(const QString &value)
{
    m_key = value;
}
