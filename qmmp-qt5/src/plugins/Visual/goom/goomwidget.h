/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/
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
