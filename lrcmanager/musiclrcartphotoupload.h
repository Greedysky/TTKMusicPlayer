#ifndef MUSICLRCARTPHOTOUPLOAD_H
#define MUSICLRCARTPHOTOUPLOAD_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicmovedialogabstract.h"

namespace Ui {
class MusicLrcArtPhotoUpload;
}

class MUSIC_WIDGET_EXPORT MusicLrcArtPhotoUpload : public MusicMoveDialogAbstract
{
    Q_OBJECT
public:
    explicit MusicLrcArtPhotoUpload(QWidget *parent = 0);

signals:
public slots:
    void selectButtonClicked();
    void uploadButtonClicked();

protected:
    Ui::MusicLrcArtPhotoUpload *ui;

};

#endif // MUSICLRCARTPHOTOUPLOAD_H
