#ifndef MUSICMOVEWIDGETABSTRACT_H
#define MUSICMOVEWIDGETABSTRACT_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include <QMouseEvent>
#include "musiclibexportglobal.h"

class MUSIC_WIDGET_EXPORT MusicMoveWidgetAbstract : public QWidget
{
    Q_OBJECT
public:
    explicit MusicMoveWidgetAbstract(QWidget *parent = 0);
    ~MusicMoveWidgetAbstract();

    void drawWindowRoundedRect(QWidget *widget);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    QPoint m_pressAt;
    bool m_leftButtonPress;

};

#endif // MUSICMOVEWIDGETABSTRACT_H
