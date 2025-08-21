#ifndef MUSICREMOTEWIDGETFORCOMPLEXSTYLE_H
#define MUSICREMOTEWIDGETFORCOMPLEXSTYLE_H

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

#include "musicremotewidget.h"

/*! @brief The class of the desktop complex remote widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicRemoteWidgetForComplexStyle : public MusicRemoteWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicRemoteWidgetForComplexStyle(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicRemoteWidgetForComplexStyle();

    /*!
     * Set current song text.
     */
    virtual void setLabelText(const QString &value) override final;

private:
    /*!
     * Show artist small picture, if no exsit there is default pic.
     */
    bool showArtPicture(const QString &name);

    QLabel *m_iconLabel, *m_songName, *m_songArtist;

};

#endif // MUSICREMOTEWIDGETFORCOMPLEXSTYLE_H
