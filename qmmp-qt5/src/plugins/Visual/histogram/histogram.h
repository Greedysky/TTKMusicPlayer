/***************************************************************************
 *   Copyright (C) 2007-2015 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include <qmmp/visual.h>

class QTimer;
class QMenu;
class QPainter;
class QPaintEvent;
class QHideEvent;
class QShowEvent;

class Histogram : public Visual
{
    Q_OBJECT
public:
    Histogram( QWidget *parent = 0);
    virtual ~Histogram();

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
    double *m_intern_vis_data;
    int *m_x_scale, m_buffer_at, m_cols, m_rows;
    double m_analyzer_falloff;
    float *m_left_buffer;
    QSize m_cell_size;

};


#endif
