/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (C) 2015 - 2018 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#ifndef PLUSVOLUMEWAVE_H
#define PLUSVOLUMEWAVE_H

#include <QWidget>
#include <qmmp/visual.h>

class QTimer;
class QPainter;
class QPaintEvent;
class QHideEvent;
class QShowEvent;

class PlusVolumeWave : public Visual
{
    Q_OBJECT
public:
    PlusVolumeWave( QWidget *parent = 0);
    virtual ~PlusVolumeWave();

public slots:
    void start();
    void stop();

private slots:
    void timeout();

private:
    void clear();
    virtual void hideEvent (QHideEvent *e);
    virtual void showEvent (QShowEvent *e);
    void paintEvent(QPaintEvent *e);

    void process();
    void draw(QPainter *p);

    QTimer *m_timer;
    double *m_intern_vis_data;
    double m_analyzer_falloff;
    float m_left_buffer[QMMP_VISUAL_NODE_SIZE];
    float m_right_buffer[QMMP_VISUAL_NODE_SIZE];
    int *m_x_scale, m_cols, m_rows;
    bool m_running;

};

#endif
