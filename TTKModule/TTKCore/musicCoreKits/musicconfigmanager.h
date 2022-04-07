#ifndef MUSICCONFIGMANAGER_H
#define MUSICCONFIGMANAGER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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
 ***************************************************************************/

#include <QColor>
#include "musicabstractxml.h"

/*! @brief The class of the xml config manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicConfigManager : public MusicAbstractXml
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicConfigManager)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicConfigManager(QObject *parent = nullptr);

    /*!
     * Read datas from xml file.
     */
    inline bool fromFile() { return MusicAbstractXml::fromFile(COFIG_PATH_FULL); }

    /*!
     * Read datas from config file.
     */
    void readBuffer() const;
    /*!
     * Write datas into config file.
     */
    void writeBuffer();

    /*!
     * Read window widget Geometry Config.
     */
    QRect readWindowGeometry() const;
    /*!
     * Read Show Desktop Lrc Geometry Config.
     */
    QPoint readShowDesktopLrcGeometry() const;

};

#endif // MUSICCONFIGMANAGER_H
