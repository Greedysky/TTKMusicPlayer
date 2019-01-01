#ifndef MUSICMOBILEDEVICESWIDGET_H
#define MUSICMOBILEDEVICESWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

#include <QLabel>
#include "musicobject.h"
#include "musicglobaldefine.h"

class QToolButton;

/*! @brief The class of the mobile devices widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicMobileDevicesWidget : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicMobileDevicesWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicMobileDevicesWidget(QWidget *parent = nullptr);

    ~MusicMobileDevicesWidget();

public Q_SLOTS:
    /*!
     * Show mobile manager widget.
     */
    void showMobileManager();

protected:
    QToolButton *m_closeButton;
    QToolButton *m_openButton;

};

#endif // MUSICMOBILEDEVICESWIDGET_H
