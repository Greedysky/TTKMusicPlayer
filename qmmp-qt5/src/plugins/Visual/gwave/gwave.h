/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/
#ifndef GWAVE_H
#define GWAVE_H

#include <QWidget>
#include <qmmp/visual.h>

class QTimer;
class QPainter;
class QPaintEvent;
class QHideEvent;
class QShowEvent;

class GWave : public Visual
{
    Q_OBJECT
public:
    GWave( QWidget *parent = 0);
    virtual ~GWave();

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

    void process(float *l, float *r);
    void draw(QPainter *p);

    QList<QColor> m_colors;
    QTimer *m_timer;
    double *m_intern_vis_data, *m_peaks;
    int *m_x_scale;
    double m_peaks_falloff, m_analyzer_falloff;
    float *m_left_buffer, *m_right_buffer;
    int m_buffer_at, m_cols, m_rows;

    QSize m_cell_size;

};

#endif
