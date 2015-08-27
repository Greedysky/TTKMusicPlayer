#ifndef MUSICSONGSLISTITEM_H
#define MUSICSONGSLISTITEM_H

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
class MusicSongsListItemInformation;
}

class MUSIC_WIDGET_EXPORT MusicSongsListItemInformation : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSongsListItemInformation(QWidget *parent = 0);
    ~MusicSongsListItemInformation();

    void setMusicSongInformation(QMusicSong &info);

protected:
    Ui::MusicSongsListItemInformation *ui;
    bool showArtPicture(const QString &name);

};

#endif // MUSICSONGSLISTITEM_H
