/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/
#ifndef FWAVE_H
#define FWAVE_H

#include <QWidget>
#include <qmmp/visual.h>

class QTimer;
class QMenu;
class QAction;
class QActionGroup;
class QPainter;
class QPaintEvent;
class QHideEvent;
class QShowEvent;


class EWave : public Visual
{
    Q_OBJECT

public:
    EWave( QWidget *parent = 0);
    virtual ~EWave();

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
    void mousePressEvent(QMouseEvent *e);
    void process(float *l, float *r);
    void draw(QPainter *p);
    void createMenu();

    QList<QColor> m_colors;
    QMenu *m_menu;
    QTimer *m_timer;
    double *m_intern_vis_data, *m_peaks;
    int *m_x_scale;
    double m_peaks_falloff, m_analyzer_falloff;
    float *m_left_buffer, *m_right_buffer;
    int m_buffer_at, m_cols, m_rows;

    QSize m_cell_size;

};


#endif
