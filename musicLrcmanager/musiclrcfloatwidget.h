#ifndef MUSICLRCFLOATWIDGET_H
#define MUSICLRCFLOATWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclrcfloatabstractwidget.h"

class QPushButton;
class MusicLrcFloatPhotoWidget;
class MusicLrcFloatSettingWidget;

class MUSIC_LRC_EXPORT MusicLrcFloatWidget : public MusicLrcFloatAbstractWidget
{
    Q_OBJECT
public:
    explicit MusicLrcFloatWidget(QWidget *parent = 0);
    virtual ~MusicLrcFloatWidget();

    void resizeWidth(int width);

public slots:
    void showFloatSettingWidget();
    void closeFloatSettingWidget();

protected:
    int m_valueX;
    QPushButton *m_more,*m_update,*m_search;
    QPushButton *m_wallp,*m_photo;
    MusicLrcFloatPhotoWidget *m_floatPhotoWidget;
    MusicLrcFloatSettingWidget *m_floatSettingWidget;

};

#endif // MUSICLRCFLOATWIDGET_H
