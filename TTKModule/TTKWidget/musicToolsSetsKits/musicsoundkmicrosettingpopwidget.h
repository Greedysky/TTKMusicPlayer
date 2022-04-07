#ifndef MUSICSOUNDKMICROSETTINGPOPWIDGET_H
#define MUSICSOUNDKMICROSETTINGPOPWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#include "musictoolmenuwidget.h"

class QComboBox;
class MusicAudioRecorderModule;

/*! @brief The class of the sound kmicro setting pop widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSoundKMicroSettingPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSoundKMicroSettingPopWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSoundKMicroSettingPopWidget(QWidget *parent = nullptr);
    ~MusicSoundKMicroSettingPopWidget();

    /*!
     * Set audio core.
     */
    void setAudioCore(MusicAudioRecorderModule *core);
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
    void initialize();

    QComboBox *m_inputComboBox, *m_outputComboBox;
    MusicAudioRecorderModule *m_recordCore;

};

#endif // MUSICSOUNDKMICROSETTINGPOPWIDGET_H
