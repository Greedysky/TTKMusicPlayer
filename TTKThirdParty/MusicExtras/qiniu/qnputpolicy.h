#ifndef QNPUTPOLICY_H
#define QNPUTPOLICY_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicprivate.h"
#include "musicextrasglobaldefine.h"

class QNMac;
class QNPutPolicyPrivate;

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

    qint32 getFSizeLimit() const;
    void setFSizeLimit(qint32 value);

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
    TTK_DECLARE_PRIVATE(QNPutPolicy)

};

#endif // QNPUTPOLICY_H
