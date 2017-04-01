#ifndef MUSICLICENSECORE_H
#define MUSICLICENSECORE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicglobaldefine.h"

/*! @brief The class of the license core module.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicLicenseCore : public QObject
{
    Q_OBJECT
public:
    explicit MusicLicenseCore(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    bool checkLicense(const QString &str);
    /*!
     * Check license string.
     */
    QString getCharacteristicString();
    /*!
     * Get characteristic string.
     */
    QString getCharacteristicStringNormal();
    /*!
     * Get characteristic normal string.
     */
    QString getEncodeKey(const QString &key);
    /*!
     * Get encode key.
     */

private:
    QStringList getCharacteristicStrings();
    /*!
     * Get characteristic strings.
     */
    QByteArray hmacSha1(const QByteArray &data, const QByteArray &key);
    /*!
     * Get hmac in sha1.
     */
    QString splitString(const QByteArray &data, const QByteArray &key);
    /*!
     * Split hmac string.
     */

};

#endif // MUSICLICENSECORE_H
