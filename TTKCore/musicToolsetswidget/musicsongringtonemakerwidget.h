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
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongRingtoneMaker();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    void initInputPath();
    /*!
     * Selected input path.
     */
    void initOutputPath();
    /*!
     * Selected save path.
     */
    void playInputSong();
    /*!
     * Start to play input song.
     */
    void playRingtone();
    /*!
     * Start to play ringtone.
     */
    void positionChanged(qint64 position);
    /*!
     * Current position changed.
     */
    void durationChanged(qint64 duration);
    /*!
     * Current duration changed.
     */
    void posChanged(qint64 start, qint64 end);
    /*!
     * Moving button pos changed.
     */
    void buttonReleaseChanged(qint64 pos);
    /*!
     * Moving button pos release changed.
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
    void playButtonStateChanged();
    /*!
     * Play button state changed.
     */

    Ui::MusicSongRingtoneMaker *ui;
    bool m_playRingtone;
    QString m_inputFilePath;
    MusicCoreMPlayer *m_player;
    qint64 m_startPos, m_stopPos;

};

#endif // MUSICSONGRINGTONEMAKER_H
