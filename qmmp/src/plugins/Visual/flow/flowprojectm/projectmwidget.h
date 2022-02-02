/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#ifndef PROJECTMWIDGET_H
#define PROJECTMWIDGET_H

#include <QGLWidget>
#if QT_VERSION >= 0x050400
#  ifdef Q_OS_UNIX
#    include <QOpenGLWidget>
#    define QT_OPENGL_WIDGET
#  endif
#endif
#include <libprojectM/projectM.hpp>

/*!
 * @author Greedysky <greedysky@163.com>
 */
#ifdef QT_OPENGL_WIDGET
class ProjectMWidget : public QOpenGLWidget
#else
class ProjectMWidget : public QGLWidget
#endif
{
    Q_OBJECT
public:
    explicit ProjectMWidget(QWidget *parent = nullptr);
    virtual ~ProjectMWidget();

    projectM *projectMInstance();

protected:
    virtual void initializeGL() override final;
    virtual void resizeGL(int width, int height) override final;
    virtual void paintGL() override final;

public slots:
    void nextPreset();
    void previousPreset();
    void randomPreset();

private:
    projectM *m_projectM = nullptr;

};

#endif
