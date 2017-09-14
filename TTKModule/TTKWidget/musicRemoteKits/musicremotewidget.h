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
    enum RemoteType
    {
        Null = 0,       /*!< Romote Null type*/
        Circle,         /*!< Romote Circle type*/
        Square,         /*!< Romote Square type*/
        Rectangle,      /*!< Romote Rectangle type*/
        SimpleStyle,    /*!< Romote SimpleStyle type*/
        ComplexStyle,   /*!< Romote ComplexStyle type*/
        Diamond,        /*!< Romote Diamond type*/
        Ripples         /*!< Romote Ripples type*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicRemoteWidget(QWidget *parent = 0);

    virtual ~MusicRemoteWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set current play state button.
     */
    void showPlayStatus(bool status) const;
    /*!
     * Set current volume value by index.
     */
    void setVolumeValue(int index);
    /*!
     * Set current song text.
     */
    virtual void setLabelText(const QString &text);
    /*!
     * Map remote type to index.
     */
    int mapRemoteTypeIndex();

Q_SIGNALS:
    /*!
     * Set show application normal.
     */
    void musicWindowSignal();
    /*!
     * Set current play state.
     */
    void musicKeySignal();
    /*!
     * Set current play to previous.
     */
    void musicPlayPreviousSignal();
    /*!
     * Set current play to next.
     */
    void musicPlayNextSignal();
    /*!
     * Set current play volume by value.
     */
    void musicVolumeSignal(int value);
    /*!
     * Show current setting widget.
     */
    void musicSettingSignal();
    /*!
     * Change diff remote style by type.
     */
    void musicRemoteTypeChanged(QAction *type);

public Q_SLOTS:
    /*!
     * Set current play volume change by value.
     */
    void musicVolumeChanged(int value);
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
     * Adjust the widget postion.
     */
    void adjustPostion(QWidget *w);

    QPushButton *m_showMainWindow, *m_PreSongButton;
    QPushButton *m_NextSongButton, *m_PlayButton;
    QPushButton *m_SettingButton;
    QWidget *m_mainWidget, *m_volumeWidget;
    QToolButton *m_volumeButton;
    MusicClickedSlider *m_volumeSlider;

};

#endif // MUSICREMOTEWIDGET_H
