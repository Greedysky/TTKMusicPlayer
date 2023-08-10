#ifndef MUSICREMOTEWIDGETFORSTRIP_H
#define MUSICREMOTEWIDGETFORSTRIP_H

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

/*! @brief The class of the desktop strip remote widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicRemoteWidgetForStrip : public MusicRemoteWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicRemoteWidgetForStrip)
public:
    /*!
     * Object constructor.
     */
    explicit MusicRemoteWidgetForStrip(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicRemoteWidgetForStrip();

    /*!
     * Set current song text.
     */
    virtual void setLabelText(const QString &value) override final;

private Q_SLOTS:
    /*!
     * Window statec hanged.
     */
    void windowStateChanged();

private:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;

    QPushButton *m_lrcButton, *m_wallPaperButton, *m_listButton;
    QPushButton *m_windowStateButton;
    QToolButton *m_closeButton;
    MusicMarqueeWidget *m_songNameLabel;
    QWidget *m_bottomWidget;

};

#endif // MUSICREMOTEWIDGETFORSTRIP_H
