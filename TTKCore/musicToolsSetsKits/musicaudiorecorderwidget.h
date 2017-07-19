#ifndef MUSICAUDIORECORDERWIDGET_H
#define MUSICAUDIORECORDERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
public:
    explicit MusicAudioRecorderWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicAudioRecorderWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

Q_SIGNALS:
    void resetFlag(MusicObject::ToolsType flag);
    /*!
     * Reset window open flag.
     */

public Q_SLOTS:
    void onRecordStart();
    /*!
     * Recorder play start.
     */
    void onRecordStop();
    /*!
     * Recorder play stop.
     */
    void onRecordPlay();
    /*!
     * Recorder play play.
     */
    void onRecordSave();
    /*!
     * Recorder play save.
     */
    void onStateChange(QAudio::State state);
    /*!
     * Recorder play state changed.
     */
    void onReadMore();
    /*!
     * Recorder to read more data.
     */
    void onSliderValueChanged(int value);
    /*!
     * Change current volume to set to recorder.
     */
    void onTimeOut();
    /*!
     * To detect current record state value.
     */
    void onTimerout();
    /*!
     * Change current to label show.
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
    int applyVolumeToSample(short iSample);
    /*!
     * Apply volume to sample.
     */
    void initMonitor();
    /*!
     * Init parameter monitor.
     */
    void createAudioInput();
    /*!
     * Create audio input.
     */
    void createAudioOutput();
    /*!
     * Create audio output.
     */

    Ui::MusicAudioRecorderWidget *m_ui;
    QTimer m_timer;
    qint64 m_time;
    QMovie *m_movie;

    int m_miVolume;
    int m_miMaxValue;
    MusicAudioRecorderCore *m_recordCore;
    QAudioFormat m_mFormatSound;
    QAudioInput *m_mpAudioInputSound;
    QAudioOutput *m_mpAudioOutputSound;
    QIODevice *m_mpInputDevSound;
    QIODevice *m_mpOutputDevSound;
    QByteArray m_mBuffer;

};

#endif // MUSICAUDIORECORDERWIDGET_H
