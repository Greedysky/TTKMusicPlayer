#ifndef MUSICLRCFLOATSETTINGWIDGET_H
#define MUSICLRCFLOATSETTINGWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
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

    void resizeWidth(int width);

Q_SIGNALS:
    void widgetClose();

public Q_SLOTS:
    void show();
    /*!
     * Override show function.
     */
    void lrcSizeUpChanged();
    void lrcSizeLowChanged();
    void lrcMusicBackgroundChanged();
    void lrcArtBackgroundChanged();

protected:
    virtual void enterEvent(QEvent *) override {}
    virtual void leaveEvent(QEvent *event) override;
    /*!
     * Override the widget event.
     */

    QPushButton *createPushButton(int index);

};

#endif // MUSICLRCFLOATSETTINGWIDGET_H
