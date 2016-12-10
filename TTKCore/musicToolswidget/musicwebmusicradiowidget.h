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
    explicit MusicWebMusicRadioWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicWebMusicRadioWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setNetworkCookie(QNetworkCookieJar *jar);
    /*!
     * Set network cookie.
     */
    void updateRadioList(const QString &category);
    /*!
     * Update radio list by given category.
     */

public Q_SLOTS:
    void radioPlay();
    /*!
     * Set radio to play.
     */
    void radioPrevious();
    /*!
     * Set radio to previous.
     */
    void radioNext();
    /*!
     * Set radio to next.
     */
    void radioVolume(int num);
    /*!
     * Set radio volume.
     */
    void getPlayListFinished();
    /*!
     * Get playList finished.
     */
    void getSongInfoFinished();
    /*!
     * Get song information finished.
     */
    void lrcDownloadStateChanged();
    /*!
     * Lrc download state changed.
     */
    void picDownloadStateChanged();
    /*!
     * Small pic download state changed.
     */
    void positionChanged(qint64 position);
    /*!
     * Current position changed.
     */
    void durationChanged(qint64 duration);
    /*!
     * Current duration changed.
     */
    void show();
    /*!
     * Override show function.
     */

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    /*!
     * Override the widget event.
     */
    void startToPlay();
    /*!
     * Start to play music radio.
     */

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
