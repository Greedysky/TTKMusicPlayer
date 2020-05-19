#ifndef MUSICWEBFMRADIOPLAYWIDGET_H
#define MUSICWEBFMRADIOPLAYWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#include <QTimer>
#include "musicabstractmovewidget.h"

class QNetworkCookieJar;
class MusicLrcAnalysis;
class MusicCoreMPlayer;
class MusicFMRadioSongsThread;

namespace Ui {
class MusicWebFMRadioPlayWidget;
}

/*! @brief The class of the web music radio widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebFMRadioPlayWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebFMRadioPlayWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebFMRadioPlayWidget(QWidget *parent = nullptr);

    virtual ~MusicWebFMRadioPlayWidget();

    /*!
     * Update radio song by given id.
     */
    void updateRadioSong(const QString &id);

public Q_SLOTS:
    /*!
     * Media aution play error.
     */
    void mediaAutionPlayError(int code);
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
     * Radio resource download.
     */
    void radioResourceDownload();
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
     * Create core module.
     */
    void createCoreModule();
    /*!
     * Start to play music radio.
     */
    void startToPlay();

    Ui::MusicWebFMRadioPlayWidget *m_ui;
    bool m_isPlaying;
    QString m_currentID;

    MusicLrcAnalysis *m_analysis;
    MusicCoreMPlayer *m_mediaPlayer;
    MusicFMRadioSongsThread *m_songsThread;

};

#endif // MUSICWEBFMRADIOPLAYWIDGET_H
