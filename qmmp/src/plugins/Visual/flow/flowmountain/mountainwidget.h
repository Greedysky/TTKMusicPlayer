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
class MountainWidget : public QtOpenGLWidget
{
    Q_OBJECT
public:
    explicit MountainWidget(QWidget *parent = nullptr);

    virtual void addBuffer(float *left);

protected:
    float m_angle = 25.0f, m_angleSpeed = 0.05f;

};

#endif
