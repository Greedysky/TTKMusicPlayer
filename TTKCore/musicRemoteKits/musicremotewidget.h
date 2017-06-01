#ifndef MUSICREMOTEWIDGET_H
#define MUSICREMOTEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicuiobject.h"
#include "musicabstractmovewidget.h"
#ifdef MUSIC_GREATER_NEW
#   include <QtWidgets>
#else
#   include <QtGui>
#endif

class MusicClickedSlider;

/*! @brief The class of the desktop remote widget base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_REMOTE_EXPORT MusicRemoteWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicRemoteWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicRemoteWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void showPlayStatus(bool status) const;
    /*!
     * Set current play state button.
     */
    void setVolumeValue(int index);
    /*!
     * Set current volume value by index.
     */
    virtual void setLabelText(const QString &text);
    /*!
     * Set current song text.
     */

Q_SIGNALS:
    void musicWindowSignal();
    /*!
     * Set show application normal.
     */
    void musicKeySignal();
    /*!
     * Set current play state.
     */
    void musicPlayPreviousSignal();
    /*!
     * Set current play to previous.
     */
    void musicPlayNextSignal();
    /*!
     * Set current play to next.
     */
    void musicVolumeSignal(int value);
    /*!
     * Set current play volume by value.
     */
    void musicSettingSignal();
    /*!
     * Show current setting widget.
     */
    void musicRemoteTypeChanged(QAction *type);
    /*!
     * Change diff remote style by type.
     */

public Q_SLOTS:
    void musicVolumeChanged(int value);
    /*!
     * Set current play volume change by value.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */
    void adjustPostion(QWidget *w);
    /*!
     * Adjust the widget postion.
     */

    QPushButton *m_showMainWindow, *m_PreSongButton;
    QPushButton *m_NextSongButton, *m_PlayButton;
    QPushButton *m_SettingButton;
    QWidget *m_mainWidget, *m_volumeWidget;
    QToolButton *m_volumeButton;
    MusicClickedSlider *m_volumeSlider;

};

#endif // MUSICREMOTEWIDGET_H
