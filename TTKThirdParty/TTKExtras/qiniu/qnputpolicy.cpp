#include "qnputpolicy.h"
#include "qnutils.h"
#include "qnmac.h"
#include "qnconf.h"
#///QJson import
#include "qjson/serializer.h"

#include <QStringList>

class QNPutPolicyPrivate : public TTKPrivate<QNPutPolicy>
{
public:
    QNPutPolicyPrivate();

    QByteArray toJSON(bool compact);

    QString m_scope;
    qint32 m_deadline;

    qint16 m_insertOnly; //optional, default 0
    QString m_saveKey;   //optional
    QString m_endUser;   //optional

    qint32 m_fSizeLimit;  //optional

    //let qiniu server dectect file mime type
    //1. check file name extension
    //2. check key extension
    //3. check file content
    //default is 0, auto use the specified mimetype
    //or check by step 1,2,3
    //if the server cannot figure out the mimetype,
    //use application/octet-stream
    qint16 m_detectMime; //optional, default 0
    QString m_mimeLimit; //optional

    QString m_callbackUrl;   //optional
    QString m_callbackHost;  //optional
    QString m_callbackBody;  //optional
    qint16 m_callbackFetchKey; //optional

    QString m_returnUrl;     //optional
    QString m_returnBody;    //optional

    QString m_persistentOps;         //optional
    QString m_persistentNotifyUrl;   //optional
    QString m_persistentPipeline;    //optional

};

QNPutPolicyPrivate::QNPutPolicyPrivate()
{
    //default is 1970/1/1, timestamp is 0
    m_deadline = 0;
    m_detectMime = -1;
    m_fSizeLimit = -1;
    m_insertOnly = -1;
    m_callbackFetchKey = -1;
}

QByteArray QNPutPolicyPrivate::toJSON(bool compact)
{
    MVariantMap json;
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

    Q_UNUSED(compact);
    QJson::Serializer serializer;
    bool ok;
    QByteArray data = serializer.serialize(json, &ok);
    if(!ok)
    {
        data = "{}";
    }

    return data;
}



QNPutPolicy::QNPutPolicy(const QString &scope)
{
    TTK_INIT_PRIVATE;
    TTK_D(QNPutPolicy);
    d->m_scope = scope;
}

QByteArray QNPutPolicy::toJSON(bool compact)
{
    TTK_D(QNPutPolicy);
    return d->toJSON(compact);
}

QString QNPutPolicy::makeUploadToken(const QNMac *mac)
{
    TTK_D(QNPutPolicy);
    // check whether deadline set, otherwise default is one hour
    if(d->m_deadline == 0)
    {
        d->m_deadline = QNUtils::expireInSeconds(3600);
    }

    const QByteArray &putPolicyJson = toJSON();
    QString uploadToken;
    if(mac != 0)
    {
        uploadToken = mac->signWithData(putPolicyJson);
    }
    else
    {
        const QNMac macx(QNConf::ACCESS_KEY, QNConf::SECRET_KEY);
        uploadToken = macx.signWithData(putPolicyJson);
    }
    return uploadToken;
}

QString QNPutPolicy::makeDownloadToken(const QNMac *mac)
{
    TTK_D(QNPutPolicy);
    // check whether deadline set, otherwise default is one hour
    if(d->m_deadline == 0)
    {
        d->m_deadline = QNUtils::expireInSeconds(3600);
    }

    QString downloadToken;
    if(mac != 0)
    {
        downloadToken = mac->signWithData2(d->m_scope.toUtf8());
    }
    else
    {
        const QNMac macx(QNConf::ACCESS_KEY, QNConf::SECRET_KEY);
        downloadToken = macx.signWithData2(d->m_scope.toUtf8());
    }
    return downloadToken;
}

qint32 QNPutPolicy::getDeadline() const
{
    TTK_D(QNPutPolicy);
    return d->m_deadline;
}

void QNPutPolicy::setDeadline(qint32 value)
{
    TTK_D(QNPutPolicy);
    d->m_deadline = value;
}

QString QNPutPolicy::getScope() const
{
    TTK_D(QNPutPolicy);
    return d->m_scope;
}

void QNPutPolicy::setScope(const QString &value)
{
    TTK_D(QNPutPolicy);
    d->m_scope = value;
}

quint16 QNPutPolicy::getInsertOnly() const
{
    TTK_D(QNPutPolicy);
    return d->m_insertOnly;
}

void QNPutPolicy::setInsertOnly(quint16 value)
{
    TTK_D(QNPutPolicy);
    d->m_insertOnly = value;
}

QString QNPutPolicy::getSaveKey() const
{
    TTK_D(QNPutPolicy);
    return d->m_saveKey;
}

void QNPutPolicy::setSaveKey(const QString &value)
{
    TTK_D(QNPutPolicy);
    d->m_saveKey = value;
}

QString QNPutPolicy::getEndUser() const
{
    TTK_D(QNPutPolicy);
    return d->m_endUser;
}

void QNPutPolicy::setEndUser(const QString &value)
{
    TTK_D(QNPutPolicy);
    d->m_endUser = value;
}

qint32 QNPutPolicy::getFSizeLimit() const
{
    TTK_D(QNPutPolicy);
    return d->m_fSizeLimit;
}

void QNPutPolicy::setFSizeLimit(qint32 value)
{
    TTK_D(QNPutPolicy);
    d->m_fSizeLimit = value;
}

qint16 QNPutPolicy::getDetectMime() const
{
    TTK_D(QNPutPolicy);
    return d->m_detectMime;
}

void QNPutPolicy::setDetectMime(qint16 value)
{
    TTK_D(QNPutPolicy);
    d->m_detectMime = value;
}

QString QNPutPolicy::getMimeLimit() const
{
    TTK_D(QNPutPolicy);
    return d->m_mimeLimit;
}

void QNPutPolicy::setMimeLimit(const QString &value)
{
    TTK_D(QNPutPolicy);
    d->m_mimeLimit = value;
}

QString QNPutPolicy::getCallbackUrl() const
{
    TTK_D(QNPutPolicy);
    return d->m_callbackUrl;
}

void QNPutPolicy::setCallbackUrl(const QString &value)
{
    TTK_D(QNPutPolicy);
    d->m_callbackUrl = value;
}

QString QNPutPolicy::getCallbackHost() const
{
    TTK_D(QNPutPolicy);
    return d->m_callbackHost;
}

void QNPutPolicy::setCallbackHost(const QString &value)
{
    TTK_D(QNPutPolicy);
    d->m_callbackHost = value;
}

QString QNPutPolicy::getCallbackBody() const
{
    TTK_D(QNPutPolicy);
    return d->m_callbackBody;
}

void QNPutPolicy::setCallbackBody(const QString &value)
{
    TTK_D(QNPutPolicy);
    d->m_callbackBody = value;
}

qint16 QNPutPolicy::getCallbackFetchKey() const
{
    TTK_D(QNPutPolicy);
    return d->m_callbackFetchKey;
}

void QNPutPolicy::setCallbackFetchKey(qint16 value)
{
    TTK_D(QNPutPolicy);
    d->m_callbackFetchKey = value;
}

QString QNPutPolicy::getReturnUrl() const
{
    TTK_D(QNPutPolicy);
    return d->m_returnUrl;
}

void QNPutPolicy::setReturnUrl(const QString &value)
{
    TTK_D(QNPutPolicy);
    d->m_returnUrl = value;
}

QString QNPutPolicy::getReturnBody() const
{
    TTK_D(QNPutPolicy);
    return d->m_returnBody;
}

void QNPutPolicy::setReturnBody(const QString &value)
{
    TTK_D(QNPutPolicy);
    d->m_returnBody = value;
}

QString QNPutPolicy::getPersistentOps() const
{
    TTK_D(QNPutPolicy);
    return d->m_persistentOps;
}

void QNPutPolicy::setPersistentOps(const QString &value)
{
    TTK_D(QNPutPolicy);
    d->m_persistentOps = value;
}

QString QNPutPolicy::getPersistentNotifyUrl() const
{
    TTK_D(QNPutPolicy);
    return d->m_persistentNotifyUrl;
}

void QNPutPolicy::setPersistentNotifyUrl(const QString &value)
{
    TTK_D(QNPutPolicy);
    d->m_persistentNotifyUrl = value;
}

QString QNPutPolicy::getPersistentPipeline() const
{
    TTK_D(QNPutPolicy);
    return d->m_persistentPipeline;
}

void QNPutPolicy::setPersistentPipeline(const QString &value)
{
    TTK_D(QNPutPolicy);
    d->m_persistentPipeline = value;
}
