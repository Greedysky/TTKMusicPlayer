#ifndef MUSICREMOTEWIDGETFORRIPPLES_H
#define MUSICREMOTEWIDGETFORRIPPLES_H

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

#include "musicremotewidget.h"

class MusicMarqueeWidget;

/*! @brief The class of the desktop ripples remote widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_REMOTE_EXPORT MusicRemoteWidgetForRipples : public MusicRemoteWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicRemoteWidgetForRipples)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicRemoteWidgetForRipples(QWidget *parent = nullptr);

    virtual ~MusicRemoteWidgetForRipples();

    /*!
     * Set current song text.
     */
    virtual void setLabelText(const QString &value) override;

protected:
    MusicMarqueeWidget *m_songNameLabel;

};

#endif // MUSICREMOTEWIDGETFORRIPPLES_H
