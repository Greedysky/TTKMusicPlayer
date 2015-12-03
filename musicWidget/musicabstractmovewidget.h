#ifndef MUSICABSTRACTMOVEWIDGET_H
#define MUSICABSTRACTMOVEWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#define WIDTH_S  20
#define HEIGHT_S 20

#include <QWidget>
#include <QMouseEvent>
#include "musiclibexportglobal.h"

class MUSIC_WIDGET_EXPORT MusicAbstractMoveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicAbstractMoveWidget(QWidget *parent = 0);
    virtual ~MusicAbstractMoveWidget();

    inline void drawWindowShadow(bool show) { m_showShadow = show;}
    inline void blockMoveOption(bool block) { m_moveOption = block;}

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    QPoint m_pressAt;
    bool m_moveOption;
    bool m_leftButtonPress;
    bool m_showShadow;

};

#endif // MUSICABSTRACTMOVEWIDGET_H
