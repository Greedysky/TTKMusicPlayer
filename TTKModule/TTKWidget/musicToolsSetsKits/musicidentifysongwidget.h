#ifndef MUSICIDENTIFYSONGWIDGET_H
#define MUSICIDENTIFYSONGWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#include "musicglobaldefine.h"
#include "musicwidgetheaders.h"

class QMovie;
class QStackedWidget;
class MusicCoreMPlayer;
class MusicLrcAnalysis;
class MusicAudioRecorderModule;
class MusicIdentifySongRequest;

/*! @brief The class of the song identify widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicIdentifySongWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicIdentifySongWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicIdentifySongWidget();

    /*!
     * Get query song id keys.
     */
    void initialize();

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
     * Set media song to play.
     */
    void playSongClicked();
    /*!
     * Set media song to download.
     */
    void showDownloadWidget();
    /*!
     * Set media song to share.
     */
    void showSongShareWidget();
    /*!
     * Current position changed.
     */
    void positionChanged(qint64 position);

private:
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
    QStackedWidget *m_mainWidget;
    QLabel *m_detectedLabel, *m_lrcLabel;
    QMovie *m_detectedMovie;
    QPushButton *m_detectedButton;
    MusicCoreMPlayer *m_player;
    MusicLrcAnalysis *m_analysis;
    MusicAudioRecorderModule *m_recordCore;
    MusicIdentifySongRequest *m_networkRequest;
    TTK::MusicSongInformation m_info;

};

#endif // MUSICIDENTIFYSONGWIDGET_H
