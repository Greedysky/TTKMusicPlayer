#include "qnputpolicy.h"
#include "qnutils.h"
#include "qnmac.h"
#include "qnconf.h"

#ifdef MUSIC_GREATER_NEW
#   include <QJsonObject>
#   include <QJsonDocument>
#else
#   include <QMap>
#endif

QNPutPolicy::QNPutPolicy(const QString &scope)
    : m_scope(scope)
{
    //default is 1970/1/1, timestamp is 0
    m_deadline = 0;
    
    m_detectMime = -1;
    m_fSizeLimit = -1;
    m_insertOnly = -1;
    m_callbackFetchKey = -1;
}

QByteArray QNPutPolicy::toJSON(bool compact)
{
#ifdef MUSIC_GREATER_NEW
    QJsonObject json;
#else
    QMap<QString, QVariant> json;
#endif
    json["scope"] = m_scope;
    json["deadline"] = m_deadline;

    if(m_insertOnly != -1)
    {
        json["insertOnly"] = m_insertOnly;
    }
    if(m_fSizeLimit!=-1)
    {
        json["fSizeLimit"] = m_fSizeLimit;
    }
    if(m_detectMime!=-1)
    {
        json["detectMime"] = m_detectMime;
    }
    if(m_callbackFetchKey!=-1)
    {
        json["callbackFetchKey"] = m_callbackFetchKey;
    }
    if(QNUtils::isParamValid(m_saveKey))
    {
        json["saveKey"] = m_saveKey;
    }
    if(QNUtils::isParamValid(m_endUser))
    {
        json["endUser"] = m_endUser;
    }
    if(QNUtils::isParamValid(m_mimeLimit))
    {
        json["mimeLimit"] = m_mimeLimit;
    }
    if(QNUtils::isParamValid(m_callbackUrl))
    {
        json["callbackUrl"] = m_callbackUrl;
    }
    if(QNUtils::isParamValid(m_callbackHost))
    {
        json["callbackHost"] = m_callbackHost;
    }
    if(QNUtils::isParamValid(m_callbackBody))
    {
        json["callbackBody"] = m_callbackBody;
    }
    if(QNUtils::isParamValid(m_returnUrl))
    {
        json["returnUrl"] = m_returnUrl;
    }
    if(QNUtils::isParamValid(m_returnBody))
    {
        json["returnBody"] = m_returnBody;
    }
    if(QNUtils::isParamValid(m_persistentOps))
    {
        json["persistentOps"] = m_persistentOps;
    }
    if(QNUtils::isParamValid(m_persistentNotifyUrl))
    {
        json["persistentNotifyUrl"] = m_persistentNotifyUrl;
    }
    if(QNUtils::isParamValid(m_persistentPipeline))
    {
        json["persistentPipeline"] = m_persistentPipeline;
    }

#ifdef MUSIC_GREATER_NEW
    QJsonDocument doc = QJsonDocument(json);
    QByteArray data = doc.toJson(compact ? QJsonDocument::Compact :
                                           QJsonDocument::Indented);
#else
    Q_UNUSED(compact);
    QByteArray data("{");
    QList<QString> keys(json.keys());
    QList<QVariant> values(json.values());
    for(int i=0; i<keys.count(); ++i)
    {
        switch(values[i].type())
        {
            case QVariant::String : data.append(QString("\"%1\":\"%2\",")
                                    .arg(keys[i]).arg(values[i].toString())); break;
            case QVariant::Int : data.append(QString("\"%1\":%2,")
                                 .arg(keys[i]).arg(values[i].toInt())); break;
            default: break;
        }
    }
    if(data.count() != 1) data.chop(1);
    data.append("}");
#endif
    return data;
}

QString QNPutPolicy::makeUploadToken(const QNMac *mac)
{
    // check whether deadline set, otherwise default is one hour
    if(m_deadline == 0)
    {
        m_deadline = QNUtils::expireInSeconds(3600);
    }
    QByteArray putPolicyJson = toJSON();
    QString uploadToken;
    if(mac != 0)
    {
        uploadToken = mac->signWithData(putPolicyJson);
    }
    else
    {
        QNMac macx = QNMac(QNConf::ACCESS_KEY, QNConf::SECRET_KEY);
        uploadToken = macx.signWithData(putPolicyJson);
    }
    return uploadToken;
}

QString QNPutPolicy::makeDownloadToken(const QNMac *mac)
{
    // check whether deadline set, otherwise default is one hour
    if(m_deadline == 0)
    {
        m_deadline = QNUtils::expireInSeconds(3600);
    }

    QString downloadToken;
    if(mac != 0)
    {
        downloadToken = mac->signWithData2(m_scope.toUtf8());
    }
    else
    {
        QNMac macx = QNMac(QNConf::ACCESS_KEY, QNConf::SECRET_KEY);
        downloadToken = macx.signWithData2(m_scope.toUtf8());
    }
    return downloadToken;
}

qint32 QNPutPolicy::getDeadline() const
{
    return m_deadline;
}

void QNPutPolicy::setDeadline(qint32 value)
{
    m_deadline = value;
}

QString QNPutPolicy::getScope() const
{
    return m_scope;
}

void QNPutPolicy::setScope(const QString &value)
{
    m_scope = value;
}

quint16 QNPutPolicy::getInsertOnly() const
{
    return m_insertOnly;
}

void QNPutPolicy::setInsertOnly(quint16 value)
{
    m_insertOnly = value;
}

QString QNPutPolicy::getSaveKey() const
{
    return m_saveKey;
}

void QNPutPolicy::setSaveKey(const QString &value)
{
    m_saveKey = value;
}

QString QNPutPolicy::getEndUser() const
{
    return m_endUser;
}

void QNPutPolicy::setEndUser(const QString &value)
{
    m_endUser = value;
}

qint64 QNPutPolicy::getFSizeLimit() const
{
    return m_fSizeLimit;
}

void QNPutPolicy::setFSizeLimit(qint64 value)
{
    m_fSizeLimit = value;
}

qint16 QNPutPolicy::getDetectMime() const
{
    return m_detectMime;
}

void QNPutPolicy::setDetectMime(qint16 value)
{
    m_detectMime = value;
}

QString QNPutPolicy::getMimeLimit() const
{
    return m_mimeLimit;
}

void QNPutPolicy::setMimeLimit(const QString &value)
{
    m_mimeLimit = value;
}

QString QNPutPolicy::getCallbackUrl() const
{
    return m_callbackUrl;
}

void QNPutPolicy::setCallbackUrl(const QString &value)
{
    m_callbackUrl = value;
}

QString QNPutPolicy::getCallbackHost() const
{
    return m_callbackHost;
}

void QNPutPolicy::setCallbackHost(const QString &value)
{
    m_callbackHost = value;
}

QString QNPutPolicy::getCallbackBody() const
{
    return m_callbackBody;
}

void QNPutPolicy::setCallbackBody(const QString &value)
{
    m_callbackBody = value;
}

qint16 QNPutPolicy::getCallbackFetchKey() const
{
    return m_callbackFetchKey;
}

void QNPutPolicy::setCallbackFetchKey(qint16 value)
{
    m_callbackFetchKey = value;
}

QString QNPutPolicy::getReturnUrl() const
{
    return m_returnUrl;
}

void QNPutPolicy::setReturnUrl(const QString &value)
{
    m_returnUrl = value;
}

QString QNPutPolicy::getReturnBody() const
{
    return m_returnBody;
}

void QNPutPolicy::setReturnBody(const QString &value)
{
    m_returnBody = value;
}

QString QNPutPolicy::getPersistentOps() const
{
    return m_persistentOps;
}

void QNPutPolicy::setPersistentOps(const QString &value)
{
    m_persistentOps = value;
}

QString QNPutPolicy::getPersistentNotifyUrl() const
{
    return m_persistentNotifyUrl;
}

void QNPutPolicy::setPersistentNotifyUrl(const QString &value)
{
    m_persistentNotifyUrl = value;
}

QString QNPutPolicy::getPersistentPipeline() const
{
    return m_persistentPipeline;
}

void QNPutPolicy::setPersistentPipeline(const QString &value)
{
    m_persistentPipeline = value;
}
