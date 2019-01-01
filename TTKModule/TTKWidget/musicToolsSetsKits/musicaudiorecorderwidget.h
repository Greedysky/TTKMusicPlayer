#ifndef MUSICAUDIORECORDERWIDGET_H
#define MUSICAUDIORECORDERWIDGET_H

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

#include <QTimer>
#include <QAudioInput>
#include <QAudioOutput>
#include <QMouseEvent>
#include <QAudioDeviceInfo>
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicabstractmovewidget.h"

#define BufferSize 14096

class QMovie;

namespace Ui {
class MusicAudioRecorderWidget;
}
class MusicAudioRecorderCore;

/*! @brief The class of the audio recorder widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicAudioRecorderWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAudioRecorderWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicAudioRecorderWidget(QWidget *parent = nullptr);

    virtual ~MusicAudioRecorderWidget();

public Q_SLOTS:
    /*!
     * Recorder play start.
     */
    void onRecordStart();
    /*!
     * Recorder play stop.
     */
    void onRecordStop();
    /*!
     * Recorder play play.
     */
    void onRecordPlay();
    /*!
     * Recorder play save.
     */
    void onRecordSave();
    /*!
     * Recorder play state changed.
     */
    void onStateChange(QAudio::State state);
    /*!
     * Recorder to read more data.
     */
    void onReadMore();
    /*!
     * Change current volume to set to recorder.
     */
    void onSliderValueChanged(int value);
    /*!
     * To detect current record state value.
     */
    void onTimeOut();
    /*!
     * Change current to label show.
     */
    void onTimerout();
    /*!
     * Override show function.
     */
    void show();

protected:
    /*!
     * Apply volume to sample.
     */
    int applyVolumeToSample(short iSample);
    /*!
     * Init parameter monitor.
     */
    void initMonitor();
    /*!
     * Create audio input.
     */
    void createAudioInput();
    /*!
     * Create audio output.
     */
    void createAudioOutput();

    Ui::MusicAudioRecorderWidget *m_ui;
    QTimer m_timer;
    qint64 m_time;
    QMovie *m_movie;

    int m_miVolume, m_miMaxValue;
    MusicAudioRecorderCore *m_recordCore;
    QAudioFormat m_mFormatSound;
    QAudioInput *m_mpAudioInputSound;
    QAudioOutput *m_mpAudioOutputSound;
    QIODevice *m_mpInputDevSound, *m_mpOutputDevSound;
    QByteArray m_mBuffer;

};

#endif // MUSICAUDIORECORDERWIDGET_H
