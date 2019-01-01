#ifndef MUSICIDENTIFYSONGSWIDGET_H
#define MUSICIDENTIFYSONGSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
 ================================================= */

#include "musicobject.h"
#include "musicglobaldefine.h"
#include "musicwidgetheaders.h"

class QMovie;
class QStackedWidget;
class MusicCoreMPlayer;
class MusicLrcAnalysis;
class MusicAudioRecorderCore;
class MusicIdentifySongsThread;

/*! @brief The class of the song identify widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicIdentifySongsWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicIdentifySongsThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicIdentifySongsWidget(QWidget *parent = nullptr);

    ~MusicIdentifySongsWidget();

    /*!
     * Get query song id keys.
     */
    void getKey();

public Q_SLOTS:
    /*!
     * Detected the song button clicked.
     */
    void detectedButtonClicked();
    /*!
     * Retected the song button clicked.
     */
    void reDetectButtonClicked();
    /*!
     * Detected the song finished.
     */
    void detectedTimeOut();
    /*!
     * Set music song to play.
     */
    void musicSongPlay();
    /*!
     * Set music song to download.
     */
    void musicSongDownload();
    /*!
     * Set music song to share.
     */
    void musicSongShare();
    /*!
     * Current position changed.
     */
    void positionChanged(qint64 position);

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Create the detected widget.
     */
    void createDetectedWidget();
    /*!
     * Create the detected successed widget.
     */
    void createDetectedSuccessedWidget();
    /*!
     * Create the detected failed widget.
     */
    void createDetectedFailedWidget();

    QTimer *m_timer;
    QStackedWidget *m_mainWindow;
    QLabel *m_detectedLabel, *m_lrcLabel;
    QMovie *m_detectedMovie;
    QPushButton *m_detectedButton;
    MusicCoreMPlayer *m_mediaPlayer;
    MusicLrcAnalysis *m_analysis;
    MusicAudioRecorderCore *m_recordCore;
    MusicIdentifySongsThread *m_detectedThread;
    MusicObject::MusicSongInformation m_currentSong;

};

#endif // MUSICIDENTIFYSONGSWIDGET_H
