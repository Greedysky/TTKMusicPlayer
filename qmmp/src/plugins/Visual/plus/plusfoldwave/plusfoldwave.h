/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#ifndef PLUSFOLDWAVE_H
#define PLUSFOLDWAVE_H

#include "colorwidget.h"
#include <qmmp/visual.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class PlusFoldWave : public Visual
{
    Q_OBJECT
public:
    explicit PlusFoldWave(QWidget *parent = nullptr);
    virtual ~PlusFoldWave();

public slots:
    virtual void start() override final;
    virtual void stop() override final;

private slots:
    void starTimeout();
    void readSettings();
    void writeSettings();
    void changeColor();
    void changeStarState(bool state);
    void changeStarColor();

private:
    virtual void hideEvent(QHideEvent *e) override final;
    virtual void showEvent(QShowEvent *e) override final;
    virtual void paintEvent(QPaintEvent *e) override final;
    virtual void contextMenuEvent(QContextMenuEvent *e) override final;

    virtual void process(float *left, float *right) override final;
    void draw(QPainter *p);

    QList<QColor> m_colors;
    QAction *m_starAction;
    QColor m_starColor;
    QList<StarPoint*> m_starPoints;
    QTimer *m_starTimer;
    double m_analyzer_falloff = 1.2;
    int *m_x_scale = nullptr;
    QSize m_cell_size = QSize(3, 2);

};

#endif
