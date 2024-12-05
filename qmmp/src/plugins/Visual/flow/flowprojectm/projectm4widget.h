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

#ifndef PROJECTM4WIDGET_H
#define PROJECTM4WIDGET_H

#include <QtGlobal>
#include <QListWidget>
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
#include <projectM-4/types.h>
#include <projectM-4/playlist_core.h>

/*!
 * @author Greedysky <greedysky@163.com>
 */
class ProjectMWidget : public QtOpenGLWidget
{
    Q_OBJECT
public:
    explicit ProjectM4Widget(QListWidget *widget, QWidget *parent = nullptr);
    virtual ~ProjectM4Widget();

    void addPCM(float *left, float *right);

protected:
    virtual void initializeGL() override final;
    virtual void resizeGL(int width, int height) override final;
    virtual void paintGL() override final;

public slots:
    void nextPreset();
    void previousPreset();
    void setShuffle(bool enabled);
    void lockPreset(bool lock);
    void selectPreset(int index);

private:
    static void presetSwitchedEvent(bool isHardCut, unsigned int index, void *data);

    projectm_handle m_handle = nullptr;
    projectm_playlist_handle m_playlistHandle = nullptr;
    QListWidget *m_itemWidget;

};

#endif
