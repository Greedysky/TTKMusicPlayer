#ifndef MUSICLRCMANAGERFORINLINE_H
#define MUSICLRCMANAGERFORINLINE_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclrcmanager.h"

class MUSIC_WIDGET_EXPORT MusicLRCManagerForInline : public MusicLRCManager
{
    Q_OBJECT
public:
    explicit MusicLRCManagerForInline(QWidget *parent = 0);
    ~MusicLRCManagerForInline();

    inline void setFontSize(int size)
            { m_gradientFontSize = size;}
    inline void setTransparent(int transparent)
            { m_gradientTransparent = transparent;}
    inline void setUpdateLrc(bool status)
            { m_UpdateLrc = status;}

protected:
    virtual void paintEvent(QPaintEvent *event);
    int m_gradientFontSize;
    int m_gradientTransparent;
    bool m_UpdateLrc;

};

#endif // MUSICLRCMANAGERFORINLINE_H
