/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#ifndef NORMALANALYZER_H
#define NORMALANALYZER_H

#include <qmmp/visual.h>
#include "colorwidget.h"

class QTimer;
class QMenu;
class QAction;
class QActionGroup;
class QPainter;
class QPaintEvent;
class QHideEvent;
class QShowEvent;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class NormalAnalyzer : public Visual
{
    Q_OBJECT
public:
    explicit NormalAnalyzer(QWidget *parent = nullptr);
    virtual ~NormalAnalyzer();

public slots:
    virtual void start() override;
    virtual void stop() override;

private slots:
    void timeout();
    void starTimeout();
    void readSettings();
    void writeSettings();
    void changeColor();
    void changeStarState(bool state);
    void changeStarColor();

private:
    void clear();
    virtual void hideEvent(QHideEvent *e) override;
    virtual void showEvent(QShowEvent *e) override;
    virtual void paintEvent(QPaintEvent *) override;
    virtual void mousePressEvent(QMouseEvent *e) override;

    void process();
    void draw(QPainter *p);
    void createMenu();
    
    QAction *m_starAction;
    QColor m_starColor;
    QList<StarPoint*> m_starPoints;
    QTimer *m_timer, *m_starTimer;
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
