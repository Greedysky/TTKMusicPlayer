#ifndef MUSICLRCFLOATABSTRACTWIDGET_H
#define MUSICLRCFLOATABSTRACTWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include <QPropertyAnimation>
#include "musicobject.h"
#include "musicuiobject.h"
#include "musiclibexportglobal.h"

class MUSIC_LRC_EXPORT MusicLrcFloatAbstractWidget : public QLabel
{
    Q_OBJECT
public:
    explicit MusicLrcFloatAbstractWidget(QWidget *parent = 0);
    virtual ~MusicLrcFloatAbstractWidget();

    inline void setblockAnimation(bool b) { m_blockAnimation = b;}

protected:
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *){}
    virtual void mouseMoveEvent(QMouseEvent *){}
    virtual void mousePressEvent(QMouseEvent *){}
    virtual void mouseReleaseEvent(QMouseEvent *){}
    void animationIn();
    void animationOut();

    QPropertyAnimation *m_animation;
    QRect m_rectIn, m_rectOut;
    bool m_blockAnimation;

};

#endif // MUSICLRCFLOATABSTRACTWIDGET_H
