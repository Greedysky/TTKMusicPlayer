#ifndef MUSICDOWNLOADWIDGET_H
#define MUSICDOWNLOADWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicDownloadWidget;
}

class MUSIC_WIDGET_EXPORT MusicDownloadWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicDownloadWidget(QWidget *parent = 0);
    virtual ~MusicDownloadWidget();

signals:
public slots:
    virtual int exec();

protected:
    Ui::MusicDownloadWidget *ui;

};

#endif // MUSICDOWNLOADWIDGET_H
