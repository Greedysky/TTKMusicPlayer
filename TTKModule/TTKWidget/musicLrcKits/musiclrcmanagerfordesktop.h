#ifndef MUSICLRCMANAGERFORDESKTOP_H
#define MUSICLRCMANAGERFORDESKTOP_H

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

#include "musiclrcmanager.h"

/*! @brief The class of the desktop lrc manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcManagerForDesktop : public MusicLrcManager
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcManagerForDesktop)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcManagerForDesktop(QWidget *parent = nullptr);

};


/*! @brief The class of the desktop horizontal lrc manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcManagerHorizontalDesktop : public MusicLrcManagerForDesktop
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcManagerHorizontalDesktop)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcManagerHorizontalDesktop(QWidget *parent = nullptr);

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

};


/*! @brief The class of the desktop vertical lrc manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcManagerVerticalDesktop : public MusicLrcManagerForDesktop
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcManagerVerticalDesktop)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcManagerVerticalDesktop(QWidget *parent = nullptr);

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

};

#endif // MUSICLRCMANAGERFORDESKTOP_H
