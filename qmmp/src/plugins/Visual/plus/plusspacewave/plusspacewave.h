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

#ifndef PLUSSPACEWAVE_H
#define PLUSSPACEWAVE_H

#include <qmmp/visual.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class PlusSpaceWave : public Visual
{
    Q_OBJECT
public:
    explicit PlusSpaceWave(QWidget *parent = nullptr);
    virtual ~PlusSpaceWave();

private:
    virtual void paintEvent(QPaintEvent *) override;
    virtual void contextMenuEvent(QContextMenuEvent *e) override;

    virtual void process(float *left, float *right) override;
    void draw(QPainter *p);

    QAction *m_screenAction;
    int *m_intern_ray_data, *m_x_scale;
    double m_analyzer_falloff;

    QSize m_cell_size;

};

#endif
