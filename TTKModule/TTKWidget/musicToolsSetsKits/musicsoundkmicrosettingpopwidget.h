#ifndef MUSICSOUNDKMICROSETTINGPOPWIDGET_H
#define MUSICSOUNDKMICROSETTINGPOPWIDGET_H

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

#include "musictoolmenuwidget.h"

class QComboBox;
class MusicAudioRecorderCore;

/*! @brief The class of the sound kmicro setting pop widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSoundKMicroSettingPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSoundKMicroSettingPopWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSoundKMicroSettingPopWidget(QWidget *parent = nullptr);

    virtual ~MusicSoundKMicroSettingPopWidget();

    /*!
     * Set audio core.
     */
    void setAudioCore(MusicAudioRecorderCore *core);
    /*!
     * Audio input index.
     */
    int audioInputIndex() const;
    /*!
     * Audio output index.
     */
    int audioOutputIndex() const;

public Q_SLOTS:
    /*!
     * Volume changed.
     */
    void volumeChanged(int value);

protected:
    /*!
     * Create all widget in layout.
     */
    void initWidget();

    QComboBox *m_inputComboBox, *m_outputComboBox;
    MusicAudioRecorderCore *m_recordCore;

};

#endif // MUSICSOUNDKMICROSETTINGPOPWIDGET_H
