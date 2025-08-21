#ifndef MUSICSONGRINGTONEMAKER_H
#define MUSICSONGRINGTONEMAKER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicSongRingtoneMaker;
}

class MusicCoreMPlayer;

/*! @brief The class of the song ringtone maker widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongRingtoneMaker : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongRingtoneMaker(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongRingtoneMaker();

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

private:
    /*!
     * Init control parameter.
     */
    void initialize() const;
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
