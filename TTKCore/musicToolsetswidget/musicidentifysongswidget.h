#ifndef MUSICIDENTIFYSONGSWIDGET_H
#define MUSICIDENTIFYSONGSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include <QPushButton>
#include "musicobject.h"
#include "musicglobaldefine.h"

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
public:
    explicit MusicIdentifySongsWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicIdentifySongsWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void getKey();
    /*!
     * Get query song id keys.
     */

public Q_SLOTS:
    void detectedButtonClicked();
    /*!
     * Detected the song button clicked.
     */
    void reDetectButtonClicked();
    /*!
     * Retected the song button clicked.
     */
    void detectedTimeOut();
    /*!
     * Detected the song finished.
     */
    void musicSongPlay();
    /*!
     * Set music song to play.
     */
    void musicSongDownload();
    /*!
     * Set music song to download.
     */
    void musicSongShare();
    /*!
     * Set music song to share.
     */
    void positionChanged(qint64 position);
    /*!
     * Current position changed.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */
    void createDetectedWidget();
    /*!
     * Create the detected widget.
     */
    void createDetectedSuccessedWidget();
    /*!
     * Create the detected successed widget.
     */
    void createDetectedFailedWidget();
    /*!
     * Create the detected failed widget.
     */

    QTimer *m_timer;
    QStackedWidget *m_mainWindow;
    QLabel *m_detectedLabel, *m_lrcLabel;
    QMovie *m_detectedMovie;
    QPushButton *m_detectedButton;
    MusicCoreMPlayer *m_songPlayer;
    MusicLrcAnalysis *m_analysis;
    MusicAudioRecorderCore *m_recordCore;
    MusicIdentifySongsThread *m_detectedThread;
    MusicObject::MusicSongInfomation m_currentSong;

};

#endif // MUSICIDENTIFYSONGSWIDGET_H
