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

#ifndef MADSPINWIDGET_H
#define MADSPINWIDGET_H

#include <QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
#  if QT_VERSION >= QT_VERSION_CHECK(6,0,0) || defined Q_OS_UNIX
#    include <QOpenGLWidget>
#    define QtOpenGLWidget QOpenGLWidget
#  else
#    include <QGLWidget>
#    define QtOpenGLWidget QGLWidget
#  endif
#else
#  if defined Q_OS_WIN && defined QT_OPENGL_ES_2
#    undef QT_OPENGL_ES_2
#  endif
#  include <QGLWidget>
#  define QtOpenGLWidget QGLWidget
#endif

/*!
 * @author Greedysky <greedysky@163.com>
 */
class MadSpinWidget : public QtOpenGLWidget
{
    Q_OBJECT
public:
    explicit MadSpinWidget(QWidget *parent = nullptr);

    void addBuffer(float *left);

    inline void setStars(int v) noexcept { m_stars = v; }
    inline void setSpeed(int v) noexcept { m_speed = v; }

    inline int getStars() const noexcept { return m_stars; }
    inline int getSpeed() const noexcept { return m_speed; }

protected:
    virtual void initializeGL() override final;
    virtual void resizeGL(int width, int height) override final;
    virtual void paintGL() override final;

private:
    GLuint m_texture[2];

    float m_buffer[256];
    float m_texSize = 0.25f, m_total = 0.0f, m_frame = 0.0f;
    int m_stars = 512, m_speed = 1000;

};

#endif
