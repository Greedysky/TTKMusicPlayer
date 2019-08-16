#ifndef MUSICSOUNDTOUCHWIDGET_H
#define MUSICSOUNDTOUCHWIDGET_H

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
    TTK_DECLARE_MODULE(MusicSoundTouchWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSoundTouchWidget(QWidget *parent = nullptr);

    virtual ~MusicSoundTouchWidget();

public Q_SLOTS:
    /*!
     * Override show function.
     */
    void show();

private Q_SLOTS:
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
     * Set label text.
     */
    void setText(const QString &text);

    Ui::MusicSoundTouchWidget *m_ui;
    QProcess *m_process;
    MusicAudioRecorderCore *m_recordCore;

};

#endif // MUSICSOUNDTOUCHWIDGET_H
