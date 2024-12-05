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

#ifndef FLOWMATRIX_H
#define FLOWMATRIX_H

#include <qmmp/visual.h>

class QTimer;
class QHideEvent;
class QShowEvent;
class Matrix;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class FlowMatrix : public Visual
{
    Q_OBJECT
public:
    explicit FlowMatrix(QWidget *parent = nullptr);
    virtual ~FlowMatrix();

private:
    virtual void resizeEvent(QResizeEvent *e) override final;
    virtual void paintEvent(QPaintEvent *e) override final;
    virtual void contextMenuEvent(QContextMenuEvent *) override final;

    virtual void process(float *left, float *right) override final;
    virtual void process(bool state) override final;

    int m_x = 0, m_y = 0;
    QPixmap m_pixmap;
    QList<Matrix*> m_matrixs;

};

#endif
