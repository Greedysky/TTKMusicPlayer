#ifndef MUSICSPECTRUMWIDGET_H
#define MUSICSPECTRUMWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
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
public:
    enum Direction
    {
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

    explicit MusicSpectrumWidget(QWidget *parent = 0);
    virtual ~MusicSpectrumWidget();

Q_SIGNALS:
    void setSpectrum(HWND wnd, int w, int h, int x = 0, int y = 0);

public Q_SLOTS:
    void show();

private Q_SLOTS:
    void timeout();
    void close();

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    void region(const QPoint &cursorPoint);
    QPoint m_dragPosition;
    Direction m_direction;
    QTimer m_timer;

};

#endif // MUSICSPECTRUMWIDGET_H
