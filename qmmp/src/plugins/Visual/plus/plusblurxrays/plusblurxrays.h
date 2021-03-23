/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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

#ifndef PLUSBLURXRAYS_H
#define PLUSBLURXRAYS_H

#include <qmmp/visual.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class PlusBlurXRays : public Visual
{
    Q_OBJECT
public:
    explicit PlusBlurXRays(QWidget *parent = nullptr);
    virtual ~PlusBlurXRays();

private slots:
    void readSettings();
    void writeSettings();
    void changeColor();

private:
    virtual void paintEvent(QPaintEvent *e) override;
    virtual void contextMenuEvent(QContextMenuEvent *e) override;

    virtual void process(float *left, float *right) override;
    void blur();
    void drawLine(int x, int y1, int y2);
    void draw(QPainter *p);

    QColor m_color;
    int m_image_size = 0;
    unsigned int *m_image = nullptr, *m_corner = nullptr;

};

#endif
