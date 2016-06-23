#ifndef MUSICSONGRINGTONEMAKER_H
#define MUSICSONGRINGTONEMAKER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicSongRingtoneMaker;
}

/*! @brief The class of the song ringtone maker widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongRingtoneMaker : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicSongRingtoneMaker(QWidget *parent = 0);
    virtual ~MusicSongRingtoneMaker();

Q_SIGNALS:
public Q_SLOTS:
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    Ui::MusicSongRingtoneMaker *ui;

};

#endif // MUSICSONGRINGTONEMAKER_H
