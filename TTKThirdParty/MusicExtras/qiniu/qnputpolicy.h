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

/*! @brief The namespace of the qiniu policy.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNPutPolicy
{
public:
    QNPutPolicy(const QString &scope);
    /*!
     * Object contsructor.
     */

    QByteArray toJSON(bool compact = true);
    /*!
     * Transfer data to json.
     */
    QString makeUploadToken(const QNMac *mac = 0);
    /*!
     * Make uplaod token.
     */
    QString makeDownloadToken(const QNMac *mac = 0);
    /*!
     * Make download token.
     */

    QString getScope() const;
    /*!
     * Get scope.
     */
    void setScope(const QString &value);
    /*!
     * Set scope.
     */

    qint32 getDeadline() const;
    /*!
     * Get deadline.
     */
    void setDeadline(qint32 value);
    /*!
     * Set deadline.
     */

    quint16 getInsertOnly() const;
    /*!
     * Get insert only.
     */
    void setInsertOnly(quint16 value);
    /*!
     * Set insert only.
     */

    QString getSaveKey() const;
    /*!
     * Get save key.
     */
    void setSaveKey(const QString &value);
    /*!
     * Set save key.
     */

    QString getEndUser() const;
    /*!
     * Get end user.
     */
    void setEndUser(const QString &value);
    /*!
     * Set end user.
     */

    qint32 getFSizeLimit() const;
    /*!
     * Get file size limit.
     */
    void setFSizeLimit(qint32 value);
    /*!
     * Set file size limit.
     */

    qint16 getDetectMime() const;
    /*!
     * Get detect mime.
     */
    void setDetectMime(qint16 value);
    /*!
     * Set detect mime.
     */

    QString getMimeLimit() const;
    /*!
     * Get mime limit.
     */
    void setMimeLimit(const QString &value);
    /*!
     * Set mime limit.
     */

    QString getCallbackUrl() const;
    /*!
     * Get call back url.
     */
    void setCallbackUrl(const QString &value);
    /*!
     * Set call back url.
     */

    QString getCallbackHost() const;
    /*!
     * Get call back host.
     */
    void setCallbackHost(const QString &value);
    /*!
     * Set call back host.
     */

    QString getCallbackBody() const;
    /*!
     * Get call back body.
     */
    void setCallbackBody(const QString &value);
    /*!
     * Set call back body.
     */

    qint16 getCallbackFetchKey() const;
    /*!
     * Get call back fetch key.
     */
    void setCallbackFetchKey(qint16 value);
    /*!
     * Set call back fetch key.
     */

    QString getReturnUrl() const;
    /*!
     * Get return url.
     */
    void setReturnUrl(const QString &value);
    /*!
     * Set return url.
     */

    QString getReturnBody() const;
    /*!
     * Get return body.
     */
    void setReturnBody(const QString &value);
    /*!
     * Set return body.
     */

    QString getPersistentOps() const;
    /*!
     * Get persistent ops.
     */
    void setPersistentOps(const QString &value);
    /*!
     * Set persistent ops.
     */

    QString getPersistentNotifyUrl() const;
    /*!
     * Get persistent notify url.
     */
    void setPersistentNotifyUrl(const QString &value);
    /*!
     * Set persistent notify url.
     */

    QString getPersistentPipeline() const;
    /*!
     * Get persistent pipe line.
     */
    void setPersistentPipeline(const QString &value);
    /*!
     * Set persistent pipe line.
     */

private:
    TTK_DECLARE_PRIVATE(QNPutPolicy)

};

#endif // QNPUTPOLICY_H
