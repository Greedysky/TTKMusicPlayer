#ifndef MUSICSOUNDKMICROWIDGET_H
#define MUSICSOUNDKMICROWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovewidget.h"

namespace Ui {
class MusicSoundKMicroWidget;
}
class MusicCoreMPlayer;
class MusicLrcAnalysis;
class MusicLRCManagerForInline;
class MusicSoundKMicroSearchWidget;
class MusicAudioRecorderCore;

/*! @brief The class of the sound kmicro widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSoundKMicroWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicSoundKMicroWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSoundKMicroWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setButtonStyle(bool style) const;
    /*!
     * Set current button style.
     */
    void setStateButtonStyle(bool style) const;
    /*!
     * Set current state button style.
     */
    void startSeachKMicro(const QString &name);
    /*!
     * Start seach KMicro.
     */

public Q_SLOTS:
    void volumeChanged(int volume);
    /*!
     * Current volume changed.
     */
    void positionChanged(qint64 position);
    /*!
     * Current position changed.
     */
    void durationChanged(qint64 duration);
    /*!
     * Set current video duration.
     */
    void playFinished();
    /*!
     * Play finished.
     */
    void setPosition(int position);
    /*!
     * Set current play pos.
     */
    void playButtonChanged();
    /*!
     * Play button changed.
     */
    void stateButtonChanged();
    /*!
     * State button changed.
     */
    void tipsButtonChanged();
    /*!
     * Tips button changed.
     */
    void mvURLChanged(bool mv, const QString &url, const QString &lrcUrl);
    /*!
     * Set current media url.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Receive download byte data from net.
     */
    void updateAnimationLrc();
    /*!
     * Animation finished.
     */
    void recordButtonClicked();
    /*!
     * Record button clicked.
     */

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    /*!
     * Override the widget event.
     */
    void multiMediaChanged();
    /*!
     * Multi media changed.
     */
    void setItemStyleSheet(int index, int size, int transparent);
    /*!
     * Set per lrc line style sheet.
     */
    void recordStateChanged(bool state);
    /*!
     * Record state changed.
     */

    Ui::MusicSoundKMicroWidget *m_ui;
    qint64 m_intervalTime;
    bool m_stateButtonOn, m_queryMv;
    MusicCoreMPlayer *m_mediaPlayer;
    MusicSoundKMicroSearchWidget *m_searchWidget;
    MusicLrcAnalysis *m_analysis;
    QList<MusicLRCManagerForInline*> m_musicLrcContainer;
    MusicAudioRecorderCore *m_recordCore;

};

#endif // MUSICSOUNDKMICROWIDGET_H
