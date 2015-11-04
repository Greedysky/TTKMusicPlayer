#ifndef MUSICAUDIORECORDERWIDGET_H
#define MUSICAUDIORECORDERWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDialog>
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

class MUSIC_TOOLSET_EXPORT MusicAudioRecorderWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
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

public:
    explicit MusicAudioRecorderWidget(QWidget *parent = 0);
    virtual ~MusicAudioRecorderWidget();

signals:

public slots:
    void onRecordStart();
    void onRecordStop();
    void onRecordPlay();
    void onRecordSave();
    void onStateChange(QAudio::State state);
    void onReadMore();
    void onSliderValueChanged(int value);
    void onTimeOut();
    void onTimerout();
    virtual int exec();

protected:
    int addWavHeader(char *filename);
    int applyVolumeToSample(short iSample);
    void initMonitor();
    void createAudioInput();
    void createAudioOutput();

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
