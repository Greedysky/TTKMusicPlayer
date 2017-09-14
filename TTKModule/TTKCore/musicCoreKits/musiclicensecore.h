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
    /*!
     * Object contsructor.
     */
    explicit MusicLicenseCore(QObject *parent = 0);

    /*!
     * Check license string.
     */
    bool checkLicense(const QString &str);
    /*!
     * Get characteristic string.
     */
    QString getCharacteristicString();
    /*!
     * Get characteristic normal string.
     */
    QString getCharacteristicStringNormal();
    /*!
     * Get encode key.
     */
    QString getEncodeKey(const QString &key);

private:
    /*!
     * Get characteristic strings.
     */
    QStringList getCharacteristicStrings();
    /*!
     * Get hmac in sha1.
     */
    QByteArray hmacSha1(const QByteArray &data, const QByteArray &key);
    /*!
     * Split hmac string.
     */
    QString splitString(const QByteArray &data, const QByteArray &key);

};

#endif // MUSICLICENSECORE_H
