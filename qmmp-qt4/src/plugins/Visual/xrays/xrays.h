/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/
#ifndef XRAYS_H
#define XRAYS_H

#include <QWidget>
#include <qmmp/visual.h>

class QTimer;
class QPainter;
class QPaintEvent;
class QHideEvent;
class QShowEvent;

class XRays : public Visual
{
    Q_OBJECT
public:
    XRays( QWidget *parent = 0);
    virtual ~XRays();

public slots:
    void start();
    void stop();

private slots:
    void timeout();
    void readSettings();
    void writeSettings();
    void changeColor();

private:
    void clear();
    virtual void hideEvent (QHideEvent *e);
    virtual void showEvent (QShowEvent *e);
    void paintEvent(QPaintEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);

    void process();
    void draw(QPainter *p);

    QList<QColor> m_colors;
    QTimer *m_timer;
    float m_left_buffer[QMMP_VISUAL_NODE_SIZE];
    float m_right_buffer[QMMP_VISUAL_NODE_SIZE];
    bool m_running;
    int *m_intern_vis_data, m_cols, m_rows;

};


#endif
