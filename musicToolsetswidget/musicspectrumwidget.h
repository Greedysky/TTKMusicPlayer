#ifndef MUSICSPECTRUMWIDGET_H
#define MUSICSPECTRUMWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#define PADDING 10

#include <QTimer>
#include "musicabstractmovewidget.h"

class MUSIC_TOOLSET_EXPORT  MusicSpectrumWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    enum Direction{
        UP,
        DOWN,
        LEFT,
        RIGHT,
        LEFTTOP,
        LEFTBOTTOM,
        RIGHTBOTTOM,
        RIGHTTOP,
        NONE
    };
public:
    explicit MusicSpectrumWidget(QWidget *parent = 0);
    virtual ~MusicSpectrumWidget();

signals:
    void setSpectrum(HWND wnd, int w, int h, int x = 0, int y = 0);

public slots:
    void show();

private slots:
    void timeout();
    void close();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    void region(const QPoint &cursorPoint);
    QPoint m_dragPosition;
    Direction m_direction;
    QTimer m_timer;

};

#endif // MUSICSPECTRUMWIDGET_H
