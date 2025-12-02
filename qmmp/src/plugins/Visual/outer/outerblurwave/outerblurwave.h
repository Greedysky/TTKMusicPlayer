/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#ifndef OUTERBLURWAVE_H
#define OUTERBLURWAVE_H

#include <qmmp/visual.h>

class QGraphicsView;
class QGraphicsPolygonItem;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class OuterBlurWave : public Visual
{
    Q_OBJECT
public:
    explicit OuterBlurWave(QWidget *parent = nullptr);
    virtual ~OuterBlurWave();

public slots:
    virtual void start() override final;
    virtual void stop() override final;

private slots:
    void readSettings();

private:
    virtual void paintEvent(QPaintEvent *e) override final;
    virtual void resizeEvent(QResizeEvent *e) override final;

    virtual void processData(float *left, float *right) override final;

    QPointF viewToItemPoint(const QPoint &pt);

    QList<QColor> m_colors;
    qreal m_opacity = 1.0;
    int *m_xscale = nullptr;
    const double m_analyzerSize = 1.2;
    QSize m_cellSize = QSize(6, 2);

    QGraphicsView *m_view;
    QGraphicsPolygonItem *m_item;

};

#endif
