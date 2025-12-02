#ifndef MUSICSYSTEMTRAYMENU_H
#define MUSICSYSTEMTRAYMENU_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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
#include "musicwidgetheaders.h"

class TTKClickedSlider;

/*! @brief The class of the system tray menu widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSystemTrayMenu : public QMenu
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicSystemTrayMenu(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
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
     * Set current play state.
     */
    void setCurrentPlayState(bool state) const;
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

private:
    /*!
     * Create play widget actions.
     */
    void createPlayWidgetActions();
    /*!
     * Create volume widget actions.
     */
    void createVolumeWidgetActions();

    QLabel *m_showText;
    QToolButton *m_playButton, *m_volumeButton;
    TTKClickedSlider *m_volumeSlider;
    QAction *m_showLrcAction, *m_lockLrcAction;

};

#endif // MUSICSYSTEMTRAYMENU_H
