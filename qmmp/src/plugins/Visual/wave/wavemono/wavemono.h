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

#ifndef WAVMONO_H
#define WAVMONO_H

#include <qmmp/visual.h>

typedef enum palette {
    PALETTE_SPECTRUM,
    PALETTE_SOX,
    PALETTE_MONO,
}Palette;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class WaveMono : public Visual
{
    Q_OBJECT
public:
    explicit WaveMono(QWidget *parent = nullptr);
    virtual ~WaveMono();

private slots:
    void typeChanged(QAction *action);

private:
    virtual void paintEvent(QPaintEvent *) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    virtual void process(float *left, float *right) override;
    void draw(QPainter *p);
    void initialize();

    Palette m_palette;
    QImage m_backgroundImage;
    int m_pixPos;
    int *m_x_scale;
    double m_analyzer_falloff;

};

#endif
