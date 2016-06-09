#ifndef MUSICSOUNDTOUCHWIDGET_H
#define MUSICSOUNDTOUCHWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicSoundTouchWidget;
}
class QProcess;
class MusicAudioRecorderCore;

/*! @brief The class of the sound touch widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSoundTouchWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicSoundTouchWidget(QWidget *parent = 0);
    virtual ~MusicSoundTouchWidget();

public Q_SLOTS:
    virtual int exec();
    /*!
     * Override exec function.
     */

private Q_SLOTS:
    void analysisOutput();
    /*!
     * Analysis output by process.
     */
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

    void tempoSliderValueChanged(int value);
    void pitchSliderValueChanged(int value);
    void rateSliderValueChanged(int value);
    void playWavButtonClicked();
    void transformButtonClicked();

protected:
    Ui::MusicSoundTouchWidget *ui;
    QProcess *m_process;
    MusicAudioRecorderCore *m_recordCore;

};

#endif // MUSICSOUNDTOUCHWIDGET_H
