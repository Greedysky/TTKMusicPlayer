#ifndef MUSICCLICKEDLABEL_H
#define MUSICCLICKEDLABEL_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musiclibexportglobal.h"

class MUSIC_LRC_EXPORT MusicClickedLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MusicClickedLabel(QWidget *parent = 0);

signals:
    void clicked();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;

};

#endif // MUSICCLICKEDLABEL_H
