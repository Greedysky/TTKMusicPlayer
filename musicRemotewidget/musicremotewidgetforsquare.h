#ifndef MUSICREMOTEWIDGETFORSQUARE_H
#define MUSICREMOTEWIDGETFORSQUARE_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicremotewidget.h"

class MUSIC_REMOTE_EXPORT MusicRemoteWidgetForSquare : public MusicRemoteWidget
{
    Q_OBJECT
public:
    explicit MusicRemoteWidgetForSquare(QWidget *parent = 0);
    virtual ~MusicRemoteWidgetForSquare();

public slots:
    void enterTimeout();
    void leaveTimeout();

protected:
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    QGraphicsOpacityEffect *m_effect[4];
    QTimer m_enterTimer;
    QTimer m_leaveTimer;
    float m_interval;

};

#endif // MUSICREMOTEWIDGETFORSQUARE_H
