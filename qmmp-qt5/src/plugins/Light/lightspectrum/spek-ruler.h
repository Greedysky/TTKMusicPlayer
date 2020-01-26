/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
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

#ifndef SPEKRULER_H
#define SPEKRULER_H

#include <QObject>
#include <QPainter>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class SpekRuler
{
public:
    enum Position
    {
        TOP,
        RIGHT,
        BOTTOM,
        LEFT
    };

    typedef QString (*formatter_cb)(int unit);

    SpekRuler(
        int x, int y, Position pos, QString sample_label,
        int *factors, int min_units, int max_units, double spacing,
        double scale, double offset, formatter_cb formatter
    );

    void draw(QPainter &dc);

protected:
    void draw_tick(QPainter &dc, int tick);

    int x;
    int y;
    Position pos;
    QString sample_label;
    int *factors;
    int min_units;
    int max_units;
    double spacing;
    double scale;
    double offset;
    formatter_cb formatter;
};

#endif
