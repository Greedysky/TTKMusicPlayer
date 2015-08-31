#ifndef MUSICMOVEDIALOGABSTRACT_H
#define MUSICMOVEDIALOGABSTRACT_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDialog>
#include "musiclibexportglobal.h"

class MUSIC_WIDGET_EXPORT MusicMoveDialogAbstract : public QDialog
{
    Q_OBJECT
public:
    explicit MusicMoveDialogAbstract(QWidget *parent = 0);
    ~MusicMoveDialogAbstract();

    void drawWindowRoundedRect(QWidget *widget);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    QPoint m_pressAt;
    bool m_leftButtonPress;

};

#endif // MUSICMOVEDIALOGABSTRACT_H
