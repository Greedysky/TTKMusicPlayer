#ifndef MUSICWEBMUSICRADIOWIDGET_H
#define MUSICWEBMUSICRADIOWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTimer>
#include "musicabstractmovewidget.h"

class QNetworkCookieJar;
class MusicLrcAnalysis;
class MusicCoreMPlayer;
class MusicRadioSongsThread;
class MusicRadioPlayListThread;

namespace Ui {
class MusicWebMusicRadioWidget;
}

/*! @brief The class of the web music radio widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebMusicRadioWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebMusicRadioWidget(QWidget *parent = 0);

    virtual ~MusicWebMusicRadioWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set network cookie.
     */
    void setNetworkCookie(QNetworkCookieJar *jar);
    /*!
     * Update radio list by given category.
     */
    void updateRadioList(const QString &category);

public Q_SLOTS:
    /*!
     * Set radio to play.
     */
    void radioPlay();
    /*!
     * Set radio to previous.
     */
    void radioPrevious();
    /*!
     * Set radio to next.
     */
    void radioNext();
    /*!
     * Set radio volume.
     */
    void radioVolume(int num);
    /*!
     * Get playList finished.
     */
    void getPlayListFinished();
    /*!
     * Get song information finished.
     */
    void getSongInfoFinished();
    /*!
     * Lrc download state changed.
     */
    void lrcDownloadStateChanged();
    /*!
     * Small pic download state changed.
     */
    void picDownloadStateChanged();
    /*!
     * Current position changed.
     */
    void positionChanged(qint64 position);
    /*!
     * Current duration changed.
     */
    void durationChanged(qint64 duration);
    /*!
     * Override show function.
     */
    void show();

protected:
    /*!
     * Override the widget event.
     */
    virtual void closeEvent(QCloseEvent *event) override;
    /*!
     * Start to play music radio.
     */
    void startToPlay();

    Ui::MusicWebMusicRadioWidget *m_ui;
    int m_currentPlayListIndex;
    bool m_isPlaying;
    QTimer m_autoNextTimer;
    MusicLrcAnalysis *m_analysis;
    MusicCoreMPlayer *m_radio;
    MusicRadioPlayListThread *m_playListThread;
    MusicRadioSongsThread *m_songsThread;
    QStringList m_playListIds;

};

#endif // MUSICWEBMUSICRADIOWIDGET_H
