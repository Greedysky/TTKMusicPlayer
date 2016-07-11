#ifndef QNPUTPOLICY_H
#define QNPUTPOLICY_H

#include <QObject>
#include "musicextrasglobaldefine.h"

class QNMac;

//@see http://developer.qiniu.com/docs/v6/api/reference/security/put-policy.html
class MUSIC_EXTRAS_EXPORT QNPutPolicy
{
public:
    QNPutPolicy(const QString &scope);

    QByteArray toJSON(bool compact = true);
    QString makeUploadToken(const QNMac *mac = 0);
    QString makeDownloadToken(const QNMac *mac = 0);

    //getters & setters
    QString getScope() const;
    void setScope(const QString &value);

    qint32 getDeadline() const;
    void setDeadline(qint32 value);


    quint16 getInsertOnly() const;
    void setInsertOnly(quint16 value);

    QString getSaveKey() const;
    void setSaveKey(const QString &value);

    QString getEndUser() const;
    void setEndUser(const QString &value);

    qint64 getFSizeLimit() const;
    void setFSizeLimit(qint64 value);

    qint16 getDetectMime() const;
    void setDetectMime(qint16 value);

    QString getMimeLimit() const;
    void setMimeLimit(const QString &value);

    QString getCallbackUrl() const;
    void setCallbackUrl(const QString &value);

    QString getCallbackHost() const;
    void setCallbackHost(const QString &value);

    QString getCallbackBody() const;
    void setCallbackBody(const QString &value);

    qint16 getCallbackFetchKey() const;
    void setCallbackFetchKey(qint16 value);

    QString getReturnUrl() const;
    void setReturnUrl(const QString &value);

    QString getReturnBody() const;
    void setReturnBody(const QString &value);

    QString getPersistentOps() const;
    void setPersistentOps(const QString &value);

    QString getPersistentNotifyUrl() const;
    void setPersistentNotifyUrl(const QString &value);

    QString getPersistentPipeline() const;
    void setPersistentPipeline(const QString &value);

private:
    QString m_scope;
    qint32 m_deadline;

    qint16 m_insertOnly; //optional, default 0
    QString m_saveKey;   //optional
    QString m_endUser;   //optional

    qint64 m_fSizeLimit;  //optional

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

#endif // QNPUTPOLICY_H
