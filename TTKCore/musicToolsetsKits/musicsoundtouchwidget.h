#ifndef MUSICSOUNDTOUCHWIDGET_H
#define MUSICSOUNDTOUCHWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
class MUSIC_TOOLSET_EXPORT MusicSoundTouchWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicSoundTouchWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSoundTouchWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

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
    /*!
     * Set tempo slider value.
     */
    void pitchSliderValueChanged(int value);
    /*!
     * Set pitch slider value.
     */
    void rateSliderValueChanged(int value);
    /*!
     * Set rate slider value.
     */
    void openWavButtonClicked();
    /*!
     * Load wav file button clicked.
     */
    void transformButtonClicked();
    /*!
     * Transform button clicked.
     */
    void finished(int code);
    /*!
     * Transform finished.
     */

protected:
    void setText(const QString &text);
    /*!
     * Set label text.
     */

    Ui::MusicSoundTouchWidget *m_ui;
    QProcess *m_process;
    MusicAudioRecorderCore *m_recordCore;

};

#endif // MUSICSOUNDTOUCHWIDGET_H
