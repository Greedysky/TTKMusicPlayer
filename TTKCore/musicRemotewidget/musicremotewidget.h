#ifndef MUSICREMOTEWIDGET_H
#define MUSICREMOTEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicuiobject.h"
#include "musicabstractmovewidget.h"
#ifdef MUSIC_QT_5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif

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

    void showPlayStatus(bool status) const;
    /*!
     * Set current play state button.
     */
    void setVolumeValue(int index);
    /*!
     * Set current volume value by index.
     */
    virtual void setLabelText(const QString &) {}
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
    void musicVolumeSignal(int index);
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
    void musicVolumeChanged(int index);
    /*!
     * Set current play volume change by value.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    QPushButton *m_showMainWindow, *m_PreSongButton;
    QPushButton *m_NextSongButton, *m_PlayButton;
    QPushButton *m_SettingButton;
    QWidget *m_mainWidget, *m_volumeWidget;
    QLabel *m_volumeLabel;
    QSlider *m_volumeSlider;

};

#endif // MUSICREMOTEWIDGET_H
