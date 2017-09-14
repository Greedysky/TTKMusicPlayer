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
    /*!
     * Object contsructor.
     */
    explicit MusicAudioRecorderWidget(QWidget *parent = 0);

    virtual ~MusicAudioRecorderWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

Q_SIGNALS:
    /*!
     * Reset window open flag.
     */
    void resetFlag(MusicObject::ToolsType flag);

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
     * Override the widget event.
     */
    virtual void closeEvent(QCloseEvent *event) override;
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
