#ifndef MUSICSONGRINGTONEMAKER_H
#define MUSICSONGRINGTONEMAKER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicSongRingtoneMaker(QWidget *parent = 0);

    virtual ~MusicSongRingtoneMaker();

    /*!
     * Get class object name.
     */
    static QString getClassName();

public Q_SLOTS:
    /*!
     * Selected input path.
     */
    void initInputPath();
    /*!
     * Selected save path.
     */
    void initOutputPath();
    /*!
     * Start to play input song.
     */
    void playInputSong();
    /*!
     * Start to play ringtone.
     */
    void playRingtone();
    /*!
     * Current position changed.
     */
    void positionChanged(qint64 position);
    /*!
     * Current duration changed.
     */
    void durationChanged(qint64 duration);
    /*!
     * Moving button pos changed.
     */
    void posChanged(qint64 start, qint64 end);
    /*!
     * Moving button pos release changed.
     */
    void buttonReleaseChanged(qint64 pos);
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    /*!
     * Init control parameter.
     */
    void initControlParameter() const;
    /*!
     * Play button state changed.
     */
    void playButtonStateChanged();

    Ui::MusicSongRingtoneMaker *m_ui;
    bool m_playRingtone;
    QString m_inputFilePath;
    MusicCoreMPlayer *m_player;
    qint64 m_startPos, m_stopPos;

};

#endif // MUSICSONGRINGTONEMAKER_H
