/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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
 ***************************************************************************/

#ifndef NORMALANALYZER_H
#define NORMALANALYZER_H

#include "colorwidget.h"
#include <qmmp/visual.h>

class QMenu;
class QActionGroup;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class NormalAnalyzer : public Visual
{
    Q_OBJECT
public:
    explicit NormalAnalyzer(QWidget *parent = nullptr);
    virtual ~NormalAnalyzer();

private slots:
    void readSettings();
    void writeSettings();
    void changeColor();

private:
    virtual void paintEvent(QPaintEvent *e) override final;
    virtual void contextMenuEvent(QContextMenuEvent *e) override final;

    virtual void processData(float *left, float *right) override final;

    void createMenu();

    double *m_peaks = nullptr;
    int *m_xscale = nullptr;
    double m_peakSize, m_analyzerSize;
    bool m_showPeaks;

    QList<QColor> m_colors;
    const QSize m_cellSize = QSize(15, 6);

    QMenu *m_menu;
    QAction *m_peaksAction;
    QActionGroup *m_fpsActions;
    QActionGroup *m_analyzerActions, *m_peakActions;

};

#endif
