#ifndef MUSICSOUNDKMICROWIDGET_H
#define MUSICSOUNDKMICROWIDGET_H

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
#include "musicabstractmovewidget.h"

namespace Ui {
class MusicSoundKMicroWidget;
}
class MusicCoreMPlayer;
class MusicLrcAnalysis;
class MusicLrcManagerForInline;
class MusicSoundKMicroSearchWidget;
class MusicAudioRecorderCore;

/*! @brief The class of the sound kmicro widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSoundKMicroWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSoundKMicroWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSoundKMicroWidget(QWidget *parent = nullptr);

    virtual ~MusicSoundKMicroWidget();

    /*!
     * Set current button style.
     */
    void setButtonStyle(bool style) const;
    /*!
     * Set current state button style.
     */
    void setStateButtonStyle(bool style) const;
    /*!
     * Start seach KMicro.
     */
    void startSeachKMicro(const QString &name);

public Q_SLOTS:
    /*!
     * Current volume changed.
     */
    void volumeChanged(int volume);
    /*!
     * Current position changed.
     */
    void positionChanged(qint64 position);
    /*!
     * Set current video duration.
     */
    void durationChanged(qint64 duration);
    /*!
     * Play finished.
     */
    void playFinished();
    /*!
     * Set current play pos.
     */
    void setPosition(int position);
    /*!
     * Play button changed.
     */
    void playButtonChanged();
    /*!
     * State button changed.
     */
    void stateButtonChanged();
    /*!
     * Tips button changed.
     */
    void tipsButtonChanged();
    /*!
     * Set current media url.
     */
    void mediaUrlChanged(bool mv, const QString &url, const QString &lrcUrl);
    /*!
     * Receive download byte data from net.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Animation finished.
     */
    void updateAnimationLrc();
    /*!
     * Record button clicked.
     */
    void recordButtonClicked();

protected:
    /*!
     * Override the widget event.
     */
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    /*!
     * Multi media changed.
     */
    void multiMediaChanged();
    /*!
     * Set per lrc line style sheet.
     */
    void setItemStyleSheet(int index, int size, int transparent);
    /*!
     * Record state changed.
     */
    void recordStateChanged(bool state);

    Ui::MusicSoundKMicroWidget *m_ui;
    qint64 m_intervalTime;
    bool m_stateButtonOn, m_queryMovieMode;
    MusicCoreMPlayer *m_mediaPlayer;
    MusicSoundKMicroSearchWidget *m_searchWidget;
    MusicLrcAnalysis *m_analysis;
    QList<MusicLrcManagerForInline*> m_musicLrcContainer;
    MusicAudioRecorderCore *m_recordCore;

};

#endif // MUSICSOUNDKMICROWIDGET_H
