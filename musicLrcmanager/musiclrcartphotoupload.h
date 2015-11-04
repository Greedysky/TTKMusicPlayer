#ifndef MUSICLRCARTPHOTOUPLOAD_H
#define MUSICLRCARTPHOTOUPLOAD_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicLrcArtPhotoUpload;
}

class MUSIC_LRC_EXPORT MusicLrcArtPhotoUpload : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicLrcArtPhotoUpload(QWidget *parent = 0);

public slots:
    void selectButtonClicked();
    void uploadButtonClicked();
    virtual int exec();

protected:
    Ui::MusicLrcArtPhotoUpload *ui;

};

#endif // MUSICLRCARTPHOTOUPLOAD_H
