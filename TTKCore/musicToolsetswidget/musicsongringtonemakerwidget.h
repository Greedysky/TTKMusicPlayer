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

class MusicCoreMPlayer;

/*! @brief The class of the song ringtone maker widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSongRingtoneMaker : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicSongRingtoneMaker(QWidget *parent = 0);
    virtual ~MusicSongRingtoneMaker();

public Q_SLOTS:
    void initInputPath();
    /*!
     * Selected input path.
     */
    void initOutputPath();
    /*!
     * Selected save path.
     */
    void playRingtone();
    /*!
     * Start to play ringtone.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    void initControlParameter() const;
    /*!
     * Init control parameter.
     */

    Ui::MusicSongRingtoneMaker *ui;
    QString m_inputFilePath;
    MusicCoreMPlayer *m_player;

};

#endif // MUSICSONGRINGTONEMAKER_H
