#ifndef MUSICAUDIORECORDERCORE_H
#define MUSICAUDIORECORDERCORE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QFile>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include "musicglobaldefine.h"
#include "musicotherdefine.h"

/*! @brief The class of the audio recorder core.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SUPER_EXPORT MusicAudioRecorderCore : public QObject
{
    Q_OBJECT
public:
    /*! @brief The class of the audio file tag header.
     * @author Greedysky <greedysky@163.com>
     */
    typedef struct MUSIC_SUPER_EXPORT HEADER
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
    }HEADER;

    /*!
     * Object contsructor.
     */
    explicit MusicAudioRecorderCore(QObject *parent = 0);

    ~MusicAudioRecorderCore();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Add the header of wav.
     */
    int addWavHeader(const char *filename);

    /*!
     * Set volume by value.
     */
    void setVolume(int volume);
    /*!
     * Get volume by value.
     */
    int volume() const;

    /*!
     * Set output file name.
     */
    void setFileName(const QString &name);
    /*!
     * Get output file name.
     */
    QString getFileName() const;
    /*!
     * Current input device error or not.
     */
    bool error() const;

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
     * Recorder play state changed.
     */
    void onStateChange(QAudio::State state);

protected:
    int m_inputVolume;
    QAudioFormat m_mFormatFile;
    QFile *m_mpOutputFile;
    QAudioInput *m_mpAudioInputFile;
    QAudioOutput *m_mpAudioOutputFile;

};

#endif // MUSICAUDIORECORDERCORE_H
