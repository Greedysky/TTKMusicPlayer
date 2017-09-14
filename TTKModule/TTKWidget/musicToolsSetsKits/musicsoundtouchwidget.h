#ifndef MUSICSOUNDTOUCHWIDGET_H
#define MUSICSOUNDTOUCHWIDGET_H

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
class MusicSoundTouchWidget;
}
class QProcess;
class MusicAudioRecorderCore;

/*! @brief The class of the sound touch widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSoundTouchWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSoundTouchWidget(QWidget *parent = 0);

    virtual ~MusicSoundTouchWidget();

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
     * Override show function.
     */
    void show();

private Q_SLOTS:
    /*!
     * Analysis output by process.
     */
    void analysisOutput();
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
     * Set tempo slider value.
     */
    void tempoSliderValueChanged(int value);
    /*!
     * Set pitch slider value.
     */
    void pitchSliderValueChanged(int value);
    /*!
     * Set rate slider value.
     */
    void rateSliderValueChanged(int value);
    /*!
     * Load wav file button clicked.
     */
    void openWavButtonClicked();
    /*!
     * Transform button clicked.
     */
    void transformButtonClicked();
    /*!
     * Transform finished.
     */
    void finished(int code);

protected:
    /*!
     * Override the widget event.
     */
    virtual void closeEvent(QCloseEvent *event) override;
    /*!
     * Set label text.
     */
    void setText(const QString &text);

    Ui::MusicSoundTouchWidget *m_ui;
    QProcess *m_process;
    MusicAudioRecorderCore *m_recordCore;

};

#endif // MUSICSOUNDTOUCHWIDGET_H
