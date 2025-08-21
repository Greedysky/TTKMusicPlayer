#ifndef MUSICVIDEOCONTROLWIDGET_H
#define MUSICVIDEOCONTROLWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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
 ***************************************************************************/

#include "musicglobaldefine.h"
#include "musicwidgetheaders.h"
#include "musicbarragerecord.h"

class QPushButton;
class MusicVolumePopWidget;
class MusicMovingLabelSlider;
class MusicVideoQualityPopWidget;
class MusicVideoBarrageStylePopWidget;
class MusicSearchEdit;

/*! @brief The class of the video controller.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicVideoControlWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicVideoControlWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicVideoControlWidget();

    /*!
     * Set current video play value.
     */
    void setValue(qint64 position) const;
    /*!
     * Set current video duration.
     */
    void durationChanged(qint64 duration) const;
    /*!
     * Set current button style.
     */
    void setButtonStyle(bool style) const;
    /*!
     * Set current media url.
     */
    void mediaChanged(const QString &url);
    /*!
     * Resize width bound by given width and height.
     */
    void setFixedSize(int w, int h);
    /*!
     * Set quality action state enable or disable.
     */
    void setQualityActionState();

Q_SIGNALS:
    /*!
     * Set current media url by selected quality.
     */
    void mediaUrlChanged(const QString &data);
    /*!
     * Slider value changed at value.
     */
    void sliderValueChanged(int value);

    /*!
     * Open barrage on or not.
     */
    void pushBarrageChanged(bool on);
    /*!
     * Add barrage text to mv.
     */
    void addBarrageChanged(const MusicBarrageRecord &record);

public Q_SLOTS:
    /*!
     * Push barrage clicked.
     */
    void pushBarrageClicked();
    /*!
     * Send barrage clicked.
     */
    void sendBarrageClicked();

private:
    /*!
     * Create video barrage widget.
     */
    QWidget *createVideoBarrageWidget();

    QLabel *m_durationLabel;
    QPushButton *m_playButton;
    MusicMovingLabelSlider *m_timeSlider;
    MusicVolumePopWidget *m_volumeButton;
    MusicVideoQualityPopWidget *m_qualityButton;

    bool m_barrageOn;
    QPushButton *m_pushBarrage, *m_barrageSend;
    MusicVideoBarrageStylePopWidget *m_menuBarrage;
    MusicSearchEdit *m_searchEdit;

};

#endif // MUSICVIDEOCONTROLWIDGET_H
