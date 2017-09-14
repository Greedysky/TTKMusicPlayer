#ifndef MUSICSYSTEMENVIRONMENT_H
#define MUSICSYSTEMENVIRONMENT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Get class object name.
     */
    static QString getClassName();

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
