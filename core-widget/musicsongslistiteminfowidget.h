#ifndef MUSICSONGSLISTITEMINFOWIDGET_H
#define MUSICSONGSLISTITEMINFOWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicsonginformation.h"
#include "musiclibexportglobal.h"

namespace Ui {
class MusicSongsListItemInfoWidget;
}

class MUSIC_WIDGET_EXPORT MusicSongsListItemInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongsListItemInfoWidget(QWidget *parent = 0);
    ~MusicSongsListItemInfoWidget();

    void setMusicSongInformation(MusicSong &info);

protected:
    Ui::MusicSongsListItemInfoWidget *ui;
    bool showArtPicture(const QString &name);

};

#endif // MUSICSONGSLISTITEMINFOWIDGET_H
