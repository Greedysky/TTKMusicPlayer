/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/
#ifndef ANALYZER_H
#define ANALYZER_H

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

class Analyzer : public Visual
{
    Q_OBJECT
public:
    Analyzer( QWidget *parent = 0);
    virtual ~Analyzer();

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
    void mousePressEvent(QMouseEvent *e);

    void process();
    void draw(QPainter *p);
    void createMenu();
    
    QTimer *m_timer;
    double *m_intern_vis_data, *m_peaks;
    double m_peaks_falloff, m_analyzer_falloff;
    bool m_show_peaks, m_update, m_running;
    float m_left_buffer[QMMP_VISUAL_NODE_SIZE];
    float m_right_buffer[QMMP_VISUAL_NODE_SIZE];
    int *m_x_scale, m_cols, m_rows;

    //colors
    QList<QColor> m_colors;
    QSize m_cell_size;
    QMenu *m_menu;
    QAction *m_peaksAction;
    QActionGroup *m_fpsGroup;
    QActionGroup *m_analyzerFalloffGroup, *m_peaksFalloffGroup;

};

#endif
