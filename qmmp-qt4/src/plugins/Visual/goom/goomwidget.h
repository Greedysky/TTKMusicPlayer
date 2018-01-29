/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

#ifndef GOOMWIDGET_H
#define GOOMWIDGET_H

#include <QWidget>
#include <QImage>
#include <qmmp/visual.h>

extern "C" {
#include "goom.h"
}

class QTimer;
class QMenu;
class QAction;
class QActionGroup;
class QPainter;
class QPaintEvent;
class QHideEvent;
class QShowEvent;
class SoundCore;

class GoomWidget : public Visual
{
    Q_OBJECT

public:
    GoomWidget(QWidget *parent = 0);
    virtual ~GoomWidget();

public slots:
    void start();
    void stop();

private slots:
    void timeout();
    void readSettings();
    void writeSettings();
    void updateTitle();

private:
    void clear();
    virtual void hideEvent (QHideEvent *);
    virtual void showEvent (QShowEvent *);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void createMenu();

    QTimer *m_timer;
    bool m_update, m_running;
    QMenu *m_menu;
    QActionGroup *m_fpsGroup;
    QAction *m_showTitleAction;
    PluginInfo *m_goom;

    QImage m_image;
    float m_buf[2][QMMP_VISUAL_NODE_SIZE];
    gint16 m_out[2][QMMP_VISUAL_NODE_SIZE];
    int m_fps;
    SoundCore *m_core;
    QString m_title;
};


#endif
