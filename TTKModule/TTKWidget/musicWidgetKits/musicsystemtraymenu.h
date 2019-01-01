#ifndef MUSICSYSTEMTRAYMENU_H
#define MUSICSYSTEMTRAYMENU_H

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

#include "musicglobaldefine.h"
#include "musicwidgetheaders.h"

class MusicClickedSlider;

/*! @brief The class of the system tray menu widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSystemTrayMenu : public QMenu
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSystemTrayMenu)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSystemTrayMenu(QWidget *parent = nullptr);

    ~MusicSystemTrayMenu();

    /*!
     * Set tray menu song text.
     */
    void setLabelText(const QString &text) const;
    /*!
     * Set show or not desktop lrc.
     */
    void showDesktopLrc(bool show) const;
    /*!
     * Set current play state button.
     */
    void showPlayStatus(bool status) const;
    /*!
     * Set current play volume.
     */
    void setVolumeValue(int value) const;

public Q_SLOTS:
    /*!
     * Set show desktop lrc.
     */
    void showDesktopLrc();
    /*!
     * Lock current desktop lrc or not.
     */
    void lockDesktopLrc(bool lock);
    /*!
     * Lock current desktop lrc state changed.
     */
    void setWindowLockedChanged();

protected:
    /*!
     * Override the widget event.
     */
    virtual void showEvent(QShowEvent *event) override;
    /*!
     * Create play widget actions.
     */
    void createPlayWidgetActions();
    /*!
     * Create volume widget actions.
     */
    void createVolumeWidgetActions();

    QLabel *m_showText;
    QToolButton *m_PlayOrStop, *m_volumeButton;
    MusicClickedSlider *m_volumeSlider;
    QAction *m_showLrcAction, *m_lockLrcAction, *m_loginAction;

};

#endif // MUSICSYSTEMTRAYMENU_H
