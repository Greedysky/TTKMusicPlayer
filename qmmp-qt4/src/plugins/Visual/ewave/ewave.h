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

private:
    virtual void hideEvent (QHideEvent *);
    virtual void showEvent (QShowEvent *);
    void paintEvent(QPaintEvent *);
    void process(float *l, float *r);
    void draw(QPainter *p);

    QTimer *m_timer;
    double *m_intern_vis_data, *m_peaks;
    int *m_x_scale;
    double m_peaks_falloff, m_analyzer_falloff;
    float *m_left_buffer, *m_right_buffer;
    int m_buffer_at, m_cols, m_rows;

    QSize m_cell_size;

};


#endif
