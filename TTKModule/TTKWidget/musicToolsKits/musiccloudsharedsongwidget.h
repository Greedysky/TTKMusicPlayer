#ifndef MUSICCLOUDSHAREDSONGWIDGET_H
#define MUSICCLOUDSHAREDSONGWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include <QWidget>
#include "musicclouddataitem.h"

class MusicCloudToolBoxWidget;
class MusicCloudManagerWidget;

/*! @brief The class of the cloud share song widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicCloudSharedSongWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicCloudSharedSongWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicCloudSharedSongWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicCloudSharedSongWidget();

    /*!
     * Initialize cloud main widget.
     */
    void initialize() const;

private:
    MusicCloudToolBoxWidget *m_toolBoxWidget;
    MusicCloudManagerWidget *m_managerWidget;

};

#endif // MUSICCLOUDSHAREDSONGWIDGET_H
