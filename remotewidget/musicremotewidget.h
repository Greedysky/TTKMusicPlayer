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
#include "musicmovewidgetabstract.h"

class MUSIC_WIDGET_EXPORT MusicRemoteWidget : public MusicMoveWidgetAbstract
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

public slots:
    void musicVolumeChanged(int index);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void paintEvent(QPaintEvent *event);

    QPushButton *m_showMainWindow;
    QPushButton *m_PreSongButton;
    QPushButton *m_NextSongButton;
    QPushButton *m_PlayButton;
    QPushButton *m_SettingButton;
    QWidget *m_mainWidget;
    QWidget *m_volumnWidget;
    QLabel *m_volumnLabel;
    QSlider *m_volumnSlider;

};

#endif // MUSICREMOTEWIDGET_H
