#ifndef MUSICCLICKEDGROUP_H
#define MUSICCLICKEDGROUP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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

#include <QWidget>
#include "musicglobaldefine.h"

/*! @brief The class of the label widget click group mapper.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicClickedGroup : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicClickedGroup)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicClickedGroup(QObject *parent = nullptr);

    /*!
     * Map the clicked widget.
     */
    void mapped(QWidget *widget);

Q_SIGNALS:
    /*!
     * Index widget click emit.
     */
    void clicked(int index);

private Q_SLOTS:
    /*!
     * Update the current clicked state.
     */
    void update();

private:
    QList<QWidget*> m_container;

};

#endif // MUSICCLICKEDGROUP_H
