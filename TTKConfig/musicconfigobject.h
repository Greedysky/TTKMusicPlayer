#ifndef MUSICCONFIGOBJECT_H
#define MUSICCONFIGOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#include "musicobject.h"
#include "musicconfigglobaldefine.h"

/*! @brief The class of the music init object.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CONFIG_EXPORT MusicConfigObject : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicConfigObject(QObject *parent = nullptr);

    /*!
     * Check current setting file's validation.
     */
    void checkValid();

    /*!
     * Get app path.
     */
    QString getAppPath() const;
    /*!
     * Init parameters.
     */
    void initialize();
    /*!
     * Check current dir is exist, no, just create it.
     */
    void dirIsExist(const QString &name);
    /*!
     * Check the related dir is exist.
     */
    void checkTheDirectoryExist();
    /*!
     * Check the related file is exist.
     */
    void checkTheFileNeededExist();
    /*!
     * Copy file by overwrite.
     */
    void copyFileOverwrite(const QString &origin, const QString &des);
    /*!
     * Copy file.
     */
    void copyFile(const QString &origin, const QString &des);
    /*!
     * Copy linux shell file.
     */
    void copyLinuxShellFile(const QString &name, const QString &path);

};

#endif
