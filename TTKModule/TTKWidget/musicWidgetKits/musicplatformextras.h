#ifndef MUSICPLATFORMEXTRAS_H
#define MUSICPLATFORMEXTRAS_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include "musicglobaldefine.h"

class MusicAbstractPlatformExtras;

/*! @brief The class of the platform extras.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlatformExtras : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicPlatformExtras(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicPlatformExtras();

    /*!
     * Set current action state.
     */
    void setAction();
    /*!
     * Set current play state.
     */
    void setCurrentPlayState(bool state) const;

private:
    MusicAbstractPlatformExtras *m_platformExtras;

};

#endif // MUSICPLATFORMEXTRAS_H
