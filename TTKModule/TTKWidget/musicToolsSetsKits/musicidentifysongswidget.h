#ifndef MUSICIDENTIFYSONGSWIDGET_H
#define MUSICIDENTIFYSONGSWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicIdentifySongsWidget(QWidget *parent = 0);

    ~MusicIdentifySongsWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
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
    MusicCoreMPlayer *m_songPlayer;
    MusicLrcAnalysis *m_analysis;
    MusicAudioRecorderCore *m_recordCore;
    MusicIdentifySongsThread *m_detectedThread;
    MusicObject::MusicSongInformation m_currentSong;

};

#endif // MUSICIDENTIFYSONGSWIDGET_H
