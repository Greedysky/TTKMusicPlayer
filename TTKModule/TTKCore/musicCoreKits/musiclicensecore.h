#ifndef MUSICLICENSECORE_H
#define MUSICLICENSECORE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QObject>
#include "musicglobaldefine.h"

/*! @brief The class of the license core module.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicLicenseCore : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLicenseCore)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLicenseCore(QObject *parent = nullptr);

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
     * Split hmac string.
     */
    QString splitString(const QByteArray &data, const QByteArray &key);

};

#endif // MUSICLICENSECORE_H
