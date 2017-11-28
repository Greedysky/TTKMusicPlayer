/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/
#ifndef RIPPLES_H
#define RIPPLES_H

#include <QWidget>
#include <qmmp/visual.h>

class QTimer;
class QPainter;
class QPaintEvent;
class QHideEvent;
class QShowEvent;

class Ripples : public Visual
{
    Q_OBJECT
public:
    Ripples( QWidget *parent = 0);
    virtual ~Ripples();

public slots:
    void start();
    void stop();

private slots:
    void timeout();

private:
    void clear();
    virtual void hideEvent (QHideEvent *);
    virtual void showEvent (QShowEvent *);
    void paintEvent(QPaintEvent *);

    void process();
    void draw(QPainter *p);

    QTimer *m_timer;
    double *m_intern_vis_data;
    int *m_x_scale, m_cols, m_rows;
    double m_analyzer_falloff;
    float m_left_buffer[QMMP_VISUAL_NODE_SIZE];
    float m_right_buffer[QMMP_VISUAL_NODE_SIZE];
    bool m_running;

    QSize m_cell_size;

};

#endif
