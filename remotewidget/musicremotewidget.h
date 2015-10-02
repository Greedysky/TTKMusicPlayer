#ifndef MUSICREMOTEWIDGET_H
#define MUSICREMOTEWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QtWidgets>
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicabstractmovewidget.h"

class MUSIC_REMOTE_EXPORT MusicRemoteWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicRemoteWidget(QWidget *parent = 0);
    virtual ~MusicRemoteWidget();

    void showPlayStatus(bool status) const;
    void setVolumeValue(int index);
    virtual void setLabelText(const QString&){}

signals:
    void musicWindowSignal();
    void musicKeySignal();
    void musicPlayPriviousSignal();
    void musicPlayNextSignal();
    void musicVolumeSignal(int index);
    void musicSettingSignal();
    void musicRemoteTypeChanged(QAction *type);

public slots:
    void musicVolumeChanged(int index);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

    QPushButton *m_showMainWindow;
    QPushButton *m_PreSongButton;
    QPushButton *m_NextSongButton;
    QPushButton *m_PlayButton;
    QPushButton *m_SettingButton;
    QWidget *m_mainWidget;
    QWidget *m_volumeWidget;
    QLabel *m_volumeLabel;
    QSlider *m_volumeSlider;

};

#endif // MUSICREMOTEWIDGET_H
