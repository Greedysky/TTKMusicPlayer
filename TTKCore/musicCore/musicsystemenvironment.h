#ifndef MUSICSYSTEMENVIRONMENT_H
#define MUSICSYSTEMENVIRONMENT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicglobaldefine.h"

/*! @brief The class of the system environment object.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSystemEnvironment
{
public:
    MusicSystemEnvironment() = default;
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    static QString getData(const QString &data);
    /*!
     * Get variant data from system environment.
     */
    static QString getSystemDrive();
    /*!
     * Get system drive path.
     */
    static QString getSystemRoot();
    /*!
     * Get system root path.
     */
    static QString getProgramData();
    /*!
     * Get program data.
     */
    static QString getWinDir();
    /*!
     * Get win dir path.
     */
    static QString getUserName();
    /*!
     * Get user name.
     */
    static QString getPublic();
    /*!
     * Get public path.
     */
    static QString getLocalAppData();
    /*!
     * Get app data local path.
     */
    static QString getAppData();
    /*!
     * Get app data roming path.
     */
    static QString getTMP();
    /*!
     * Get temp path.
     */
    static QString getTEMP();
    /*!
     * Get temp path.
     */
    static QString getCommonProgramFiles();
    /*!
     * Get common program files path.
     */
    static QString getCommonProgramW64();
    /*!
     * Get common program w64 path.
     */
    static QString getOS();
    /*!
     * Get operator system name.
     */

};

#endif // MUSICSYSTEMENVIRONMENT_H
