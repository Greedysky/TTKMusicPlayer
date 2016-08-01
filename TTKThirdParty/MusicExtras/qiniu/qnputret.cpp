#include "qnputret.h"

#ifdef MUSIC_GREATER_NEW
#   include <QJsonDocument>
#   include <QJsonObject>
#else
#   include <QtScript/QScriptEngine>
#   include <QtScript/QScriptValue>
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
    QScriptEngine engine;
    QScriptValue sc = engine.evaluate("value=" + QString(jsonData));
    putRet->setHash(sc.property("hash").toString());
    putRet->setKey(sc.property("key").toString());
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
