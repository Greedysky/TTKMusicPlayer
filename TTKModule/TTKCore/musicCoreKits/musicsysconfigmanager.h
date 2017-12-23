#ifndef MUSICSYSCONFIGMANAGER_H
#define MUSICSYSCONFIGMANAGER_H

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

#include <QColor>
#include "musicabstractxml.h"

/*! @brief The class of the sys xml config manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicSysConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSysConfigManager(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Read config datas from xml file by given name.
     */
    inline bool readXMLConfig() { return readConfig(COFIGPATH_FULL); }
    /*!
     * Write datas into xml file.
     */
    void writeXMLConfig();

    /*!
     * Read System Last Play Index Config.
     */
    void readSystemLastPlayIndexConfig(QStringList &key) const;
    /*!
     * Read window widget Geometry Config.
     */
    QRect readWindowGeometry() const;
    /*!
     * Read Show Desktop Lrc Geometry Config.
     */
    QPoint readShowDLrcGeometry() const;
    /*!
     * Read Sys Load Config.
     */
    void readSysLoadConfig() const;

};

#endif // MUSICSYSCONFIGMANAGER_H
