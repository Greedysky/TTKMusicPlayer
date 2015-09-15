#ifndef MUSICLRCFLOATSETTINGWIDGET_H
#define MUSICLRCFLOATSETTINGWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclrcfloatabstractwidget.h"

class QPushButton;

class MUSIC_LRC_EXPORT MusicLrcFloatSettingWidget : public MusicLrcFloatAbstractWidget
{
    Q_OBJECT
public:
    explicit MusicLrcFloatSettingWidget(QWidget *parent = 0);

signals:
    void widgetClose();

public slots:
    void show();
    void lrcSizeUpChanged();
    void lrcSizeLowChanged();
    void lrcMusicBackgroundChanged();
    void lrcArtBackgroundChanged();

protected:
    virtual void enterEvent(QEvent *){}
    virtual void leaveEvent(QEvent *event);
    QPushButton *createPushButton(int index);

};

#endif // MUSICLRCFLOATSETTINGWIDGET_H
