#ifndef MUSICAUDIORECORDERWIDGET_H
#define MUSICAUDIORECORDERWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMouseEvent>
#include <QTimer>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include "musicuiobject.h"
#include "musicabstractmovedialog.h"

#define BufferSize 14096

class QMovie;

namespace Ui {
class MusicAudioRecorderWidget;
}

/*! @brief The class of the audio recorder widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicAudioRecorderWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    struct HEADER
    {
        char RIFFNAME[4];
        unsigned long nRIFFLength;
        char WAVNAME[4];
        char FMTNAME[4];
        unsigned long nFMTLength;
        unsigned short nAudioFormat;
        unsigned short nChannleNumber;
        unsigned long nSampleRate;
        unsigned long nBytesPerSecond;
        unsigned short nBytesPerSample;
        unsigned short    nBitsPerSample;
        char    DATANAME[4];
        unsigned long   nDataLength;
    };
    explicit MusicAudioRecorderWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicAudioRecorderWidget();

Q_SIGNALS:

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
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    int addWavHeader(char *filename);
    /*!
     * Add the header of wav.
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

    Ui::MusicAudioRecorderWidget *ui;
    QTimer m_timer;
    qint64 m_time;
    QMovie *m_movie;

    int m_miVolume;
    int m_miMaxValue;
    QAudioFormat m_mFormatFile;
    QFile *m_mpOutputFile;
    QAudioInput *m_mpAudioInputFile;
    QAudioOutput *m_mpAudioOutputFile;
    QAudioFormat m_mFormatSound;
    QAudioInput *m_mpAudioInputSound;
    QAudioOutput *m_mpAudioOutputSound;
    QIODevice *m_mpInputDevSound;
    QIODevice *m_mpOutputDevSound;
    QByteArray m_mBuffer;

};

#endif // MUSICAUDIORECORDERWIDGET_H
