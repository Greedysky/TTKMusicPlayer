#ifndef MUSICREMOTEWIDGETFORRIPPLE_H
#define MUSICREMOTEWIDGETFORRIPPLE_H

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

#include "musicremotewidget.h"

class MusicMarqueeWidget;

/*! @brief The class of the desktop ripple remote widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicRemoteWidgetForRipple : public MusicRemoteWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicRemoteWidgetForRipple)
public:
    /*!
     * Object constructor.
     */
    explicit MusicRemoteWidgetForRipple(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicRemoteWidgetForRipple();

    /*!
     * Set current song text.
     */
    virtual void setLabelText(const QString &value) override final;

public Q_SLOTS:
    /*!
     * Visual mode changed.
     */
    void visualModeChanged();

private:
    /*!
     * Create visual widget.
     */
    void createVisualWidget();
    /*!
     * Remove visual widget.
     */
    void removeVisualWidget();

    bool m_mode;
    QWidget *m_visual;
    QVBoxLayout *m_visualLayout;
    QPushButton *m_visualModeButton;
    MusicMarqueeWidget *m_songNameLabel;

};

#endif // MUSICREMOTEWIDGETFORRIPPLE_H
