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

#ifndef MOUNTAINWIDGET_H
#define MOUNTAINWIDGET_H

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
#  ifdef Q_OS_UNIX
#    include <QOpenGLWidget>
#    define QtOpenGLWidget QOpenGLWidget
#  else
#    include <QGLWidget>
#    define QtOpenGLWidget QGLWidget
#  endif
#else
#  include <QGLWidget>
#  define QtOpenGLWidget QGLWidget
#endif

#define NUM_BANDS 32
#define DB_RANGE 40
#define BAR_SPACING (3.2f / NUM_BANDS)
#define BAR_WIDTH   (0.8f * BAR_SPACING)

/*!
 * @author Greedysky <greedysky@163.com>
 */
class MountainWidget : public QtOpenGLWidget
{
    Q_OBJECT
public:
    explicit MountainWidget(QWidget *parent = nullptr);

    void addBuffer(float *left);

protected:
    virtual void initializeGL() override final;
    virtual void resizeGL(int width, int height) override final;
    virtual void paintGL() override final;

    void generateLogGraph(const float * freq, float * graph);
    void drawRectangle(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b);
    void drawBar(float x, float z, float h, float r, float g, float b);
    void drawBars();

private:
    float m_logScale[NUM_BANDS + 1];
    float m_colors[NUM_BANDS][NUM_BANDS][3];

    int m_offset = 0;
    float m_angle = 25, m_angleSpeed = 0.05f;
    float m_bars[NUM_BANDS][NUM_BANDS];

};

#endif
