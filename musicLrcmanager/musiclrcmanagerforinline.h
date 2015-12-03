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
#define LRC_PER_WIDTH 522

class MUSIC_LRC_EXPORT MusicLRCManagerForInline : public MusicLRCManager
{
    Q_OBJECT
public:
    explicit MusicLRCManagerForInline(QWidget *parent = 0);
    virtual ~MusicLRCManagerForInline();

    inline void setLrcPerWidth(int width){ m_lrcPerWidth = width + LRC_PER_WIDTH;}
    inline void setFontSize(int size) { m_gradientFontSize = size;}
    inline void setTransparent(int tran) { m_gradientTransparent = tran;}
    inline void setCenterOnLrc(bool status) { m_centerOnLrc = status;}

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    int m_gradientFontSize;
    int m_gradientTransparent;
    bool m_centerOnLrc;

};

#endif // MUSICLRCMANAGERFORINLINE_H
