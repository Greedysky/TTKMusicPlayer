/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

#include <QImage>
#include <qmmp/florid.h>

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

class GoomWidget : public Florid
{
    Q_OBJECT
public:
    GoomWidget(QWidget *parent = nullptr);
    virtual ~GoomWidget();

public slots:
    virtual void start() override;
    virtual void stop() override;

private slots:
    void timeout();
    void readSettings();
    void writeSettings();

private:
    void clear();
    virtual void hideEvent(QHideEvent *) override;
    virtual void showEvent(QShowEvent *) override;
    virtual void paintEvent(QPaintEvent *) override;
    virtual void mousePressEvent(QMouseEvent *e) override;
    void createMenu();

    QTimer *m_timer;
    bool m_update, m_running;
    QMenu *m_menu;
    QActionGroup *m_fpsGroup;
    PluginInfo *m_goom;

    QImage m_image;
    float m_buf[2][QMMP_VISUAL_NODE_SIZE];
    gint16 m_out[2][QMMP_VISUAL_NODE_SIZE];
    int m_fps;

};

#endif
