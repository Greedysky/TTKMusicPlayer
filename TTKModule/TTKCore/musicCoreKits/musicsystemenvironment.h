#ifndef MUSICSYSTEMENVIRONMENT_H
#define MUSICSYSTEMENVIRONMENT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

#include "musicglobaldefine.h"

/*! @brief The class of the system environment object.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSystemEnvironment
{
    TTK_DECLARE_MODULE(MusicSystemEnvironment)
public:
    /*!
     * Get variant data from system environment.
     */
    static QString getData(const QString &data);
    /*!
     * Get system drive path.
     */
    static QString getSystemDrive();
    /*!
     * Get system root path.
     */
    static QString getSystemRoot();
    /*!
     * Get program data.
     */
    static QString getProgramData();
    /*!
     * Get win dir path.
     */
    static QString getWinDir();
    /*!
     * Get user name.
     */
    static QString getUserName();
    /*!
     * Get public path.
     */
    static QString getPublic();
    /*!
     * Get app data local path.
     */
    static QString getLocalAppData();
    /*!
     * Get app data roming path.
     */
    static QString getAppData();
    /*!
     * Get temp path.
     */
    static QString getTMP();
    /*!
     * Get temp path.
     */
    static QString getTEMP();
    /*!
     * Get common program files path.
     */
    static QString getCommonProgramFiles();
    /*!
     * Get common program w64 path.
     */
    static QString getCommonProgramW64();
    /*!
     * Get operator system name.
     */
    static QString getOS();

};

#endif // MUSICSYSTEMENVIRONMENT_H
