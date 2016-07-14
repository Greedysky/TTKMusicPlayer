#include "qnputret.h"

#ifdef MUSIC_GREATER_NEW
#   include <QJsonDocument>
#   include <QJsonObject>
#else
#   include <QtScript/QScriptEngine>
#   include <QtScript/QScriptValue>
#endif

QNPutRet::QNPutRet()
{

}

QNPutRet::~QNPutRet()
{

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
    QScriptEngine engine;
    QScriptValue sc = engine.evaluate("value=" + QString(jsonData));
    putRet->setHash(sc.property("hash").toString());
    putRet->setKey(sc.property("key").toString());
#endif
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
