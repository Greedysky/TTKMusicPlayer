#ifndef MUSICSOUNDKMICROWIDGET_H
#define MUSICSOUNDKMICROWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSoundKMicroWidget(QWidget *parent = 0);

    virtual ~MusicSoundKMicroWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
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

Q_SIGNALS:
    /*!
     * Reset window open flag.
     */
    void resetFlag(MusicObject::ToolsType flag);

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
    void mvURLChanged(bool mv, const QString &url, const QString &lrcUrl);
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
    bool m_stateButtonOn, m_queryMv;
    MusicCoreMPlayer *m_mediaPlayer;
    MusicSoundKMicroSearchWidget *m_searchWidget;
    MusicLrcAnalysis *m_analysis;
    QList<MusicLrcManagerForInline*> m_musicLrcContainer;
    MusicAudioRecorderCore *m_recordCore;

};

#endif // MUSICSOUNDKMICROWIDGET_H
