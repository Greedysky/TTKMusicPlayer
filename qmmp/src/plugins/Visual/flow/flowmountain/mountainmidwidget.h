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

#ifndef MOUNTAINMIDWIDGET_H
#define MOUNTAINMIDWIDGET_H

#include "mountainwidget.h"

#ifdef NUM_BANDS
#  undef NUM_BANDS
#endif
#ifdef BAR_SPACING
#  undef BAR_SPACING
#endif

#define NUM_BANDS 32
#define DB_RANGE 40
#define BAR_SPACING (3.2f / NUM_BANDS)
#define BAR_WIDTH   (0.8f * BAR_SPACING)

/*!
 * @author Greedysky <greedysky@163.com>
 */
class MountainMidWidget : public MountainWidget
{
    Q_OBJECT
public:
    explicit MountainMidWidget(QWidget *parent = nullptr);

    void addBuffer(float *left);

protected:
    virtual void initializeGL() override final;
    virtual void resizeGL(int width, int height) override final;
    virtual void paintGL() override final;

    void generateLogGraph(const float *freq, float *graph);

    void drawBars();
    void drawBar(float x, float z, float h, float r, float g, float b);
    void drawRectangle(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b);

private:
    float m_logScale[NUM_BANDS + 1];
    float m_colors[NUM_BANDS][NUM_BANDS][3];

    int m_offset = 0;
    float m_bars[NUM_BANDS][NUM_BANDS];

};

#endif
