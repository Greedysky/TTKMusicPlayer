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

#ifndef PROJECTMWIDGET_H
#define PROJECTMWIDGET_H

#include <QtGlobal>
#include <QListWidget>
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
#include <libprojectM/projectM.hpp>

class ProjectMWrapper : public QObject, public projectM
{
    Q_OBJECT
public:
    explicit ProjectMWrapper(const Settings &settings, int flags, QObject *parent = nullptr);

signals:
    void currentPresetChanged(int index);

public slots:
    void selectPreset(int index);

private:
#ifdef PROJECTM_31
    virtual void presetSwitchedEvent(bool isHardCut, size_t index) const override final;
#else
    virtual void presetSwitchedEvent(bool isHardCut, unsigned int index) const override final;
#endif

};


/*!
 * @author Greedysky <greedysky@163.com>
 */
class ProjectMWidget : public QtOpenGLWidget
{
    Q_OBJECT
public:
    explicit ProjectMWidget(QListWidget *widget, QWidget *parent = nullptr);
    virtual ~ProjectMWidget();

    void addBuffer(float *left, float *right);

protected:
    virtual void initializeGL() override final;
    virtual void resizeGL(int width, int height) override final;
    virtual void paintGL() override final;

public slots:
    void showHelp();
    void showPresetName();
    void showTitle();
    void nextPreset();
    void previousPreset();
    void randomPreset();
    void lockPreset(bool lock);
    void setCurrentRow(int row);

private:
    QListWidget *m_itemWidget;
    ProjectMWrapper *m_projectM = nullptr;

};

#endif
