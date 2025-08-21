#ifndef MUSICREMOTEWIDGET_H
#define MUSICREMOTEWIDGET_H

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

#include <QTimer>
#include "musicwidgetutils.h"
#include "musicwidgetheaders.h"
#include "ttkabstractmovewidget.h"

class TTKClickedSlider;

/*! @brief The class of the desktop remote widget base.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicRemoteWidget : public TTKAbstractMoveWidget
{
    Q_OBJECT
public:
    enum Module
    {
        Null = 0,       /*!< Romote Null module*/
        Square,         /*!< Romote Square module*/
        Rectangle,      /*!< Romote Rectangle module*/
        SimpleStyle,    /*!< Romote SimpleStyle module*/
        ComplexStyle,   /*!< Romote ComplexStyle module*/
        Ripple          /*!< Romote Ripple module*/
    };

    /*!
     * Object constructor.
     */
    explicit MusicRemoteWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicRemoteWidget();

    /*!
     * Set current play state.
     */
    void setCurrentPlayState(bool state) const;
    /*!
     * Set current volume value by index.
     */
    void setVolumeValue(int index);
    /*!
     * Map remote type to index.
     */
    int mapRemoteTypeIndex();

    /*!
     * Set current song text.
     */
    virtual void setLabelText(const QString &text);

Q_SIGNALS:
    /*!
     * Set show application normal.
     */
    void showMainWindow();
    /*!
     * Set current play state.
     */
    void playStateChanged();
    /*!
     * Set current play to previous.
     */
    void playPreviousChanged();
    /*!
     * Set current play to next.
     */
    void playNextChanged();
    /*!
     * Set current play volume by value.
     */
    void volumeChanged(int value);
    /*!
     * Show current setting widget.
     */
    void settingChanged();
    /*!
     * Change diff remote style by type.
     */
    void remoteTypeChanged(QAction *type);

public Q_SLOTS:
    /*!
     * Set current play volume change by value.
     */
    void volumeSliderChanged(int value);
    /*!
     * Override show function.
     */
    void show();
    /*!
     * Override close function.
     */
    bool close();

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Adjust the widget position.
     */
    void adjustPosition(QWidget *w);

    QPushButton *m_showMainWindowButton, *m_preSongButton;
    QPushButton *m_nextSongButton, *m_playButton;
    QPushButton *m_settingButton;
    QWidget *m_mainWidget, *m_volumeWidget;
    QToolButton *m_volumeButton;
    TTKClickedSlider *m_volumeSlider;

};

#endif // MUSICREMOTEWIDGET_H
