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

    void add(float *data, size_t samples, int chan);
    void clear();

private slots:
    void timeout();
    void readSettings();
    void writeSettings();
    void changeColor();

private:
    virtual void hideEvent (QHideEvent *e);
    virtual void showEvent (QShowEvent *e);
    void paintEvent(QPaintEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);

    void process(float *buffer);
    void draw(QPainter *p);

    QList<QColor> m_colors;
    QTimer *m_timer;
    int *m_intern_vis_data;
    float *m_buffer;
    int m_buffer_at, m_cols, m_rows;

};


#endif
