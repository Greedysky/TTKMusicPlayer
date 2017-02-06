#ifndef MUSICWEBENTAINRADIODATEBASE_H
#define MUSICWEBENTAINRADIODATEBASE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the web entain radio database.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebEntainRadioDatabase
{
public:
    MusicWebEntainRadioDatabase();
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    bool connectDatabase() const;
    /*!
     * Connect to database.
     */
    bool disConnectDatabase() const;
    /*!
     * Disconnect to database.
     */
    QString getRadioUrl(const QString &channelName) const;
    /*!
     * Get radio url by given channelName.
     */
    QStringList getRadioNames(const QString &category) const;
    /*!
     * Get radio names by given category.
     */
    QStringList getRecentNames() const;
    /*!
     * Get recent play names.
     */
    QStringList getFavouriteNames() const;
    /*!
     * Get favourite channel names.
     */
    void radioRecentPlay(const QString &channelName) const;
    /*!
     * Set given channel name to recent play.
     */
    void radioCollection(const QString &name) const;
    /*!
     * Set given channel to collection.
     */
    void radioDiscollection(const QString &name) const;
    /*!
     * Set given channel to discollection.
     */

protected:
    QStringList getRecords(const QString &filter) const;
    /*!
     * Get records by filter.
     */

};

#endif // MUSICWEBENTAINRADIODATEBASE_H
