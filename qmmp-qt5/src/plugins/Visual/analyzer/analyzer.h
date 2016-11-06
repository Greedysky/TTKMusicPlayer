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

    void add(float *data, size_t samples, int chan);
    void clear();


private slots:
    void timeout();
    void toggleFullScreen();
    void readSettings();
    void writeSettings();


private:
    virtual void hideEvent (QHideEvent *);
    virtual void showEvent (QShowEvent *);
    virtual void closeEvent (QCloseEvent *);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void process(float *l, float *r);
    void draw(QPainter *p);
    void createMenu();
    QTimer *m_timer;
    double *m_intern_vis_data;
    double *m_peaks;
    int *m_x_scale;
    double m_peaks_falloff;
    double m_analyzer_falloff;
    bool m_show_peaks;
    float *m_left_buffer;
    float *m_right_buffer;
    int m_buffer_at;
    int m_cols, m_rows;
    bool m_update;
    //colors
    QColor m_color1;
    QColor m_color2;
    QColor m_color3;
    QColor m_bgColor;
    QColor m_peakColor;
    QSize m_cell_size;
    QMenu *m_menu;
    QAction *m_peaksAction;
    QActionGroup *m_fpsGroup;
    QActionGroup *m_analyzerFalloffGroup;
    QActionGroup *m_peaksFalloffGroup;
};


#endif
