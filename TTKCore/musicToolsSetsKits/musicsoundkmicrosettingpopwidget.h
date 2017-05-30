#ifndef MUSICSOUNDKMICROSETTINGPOPWIDGET_H
#define MUSICSOUNDKMICROSETTINGPOPWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musictoolmenuwidget.h"

class QComboBox;
class MusicAudioRecorderCore;

/*! @brief The class of the sound kmicro setting pop widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicSoundKMicroSettingPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    explicit MusicSoundKMicroSettingPopWidget(QWidget *parent = 0);

    /*!
     * Object contsructor.
     */
    virtual ~MusicSoundKMicroSettingPopWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setAudioCore(MusicAudioRecorderCore *core);
    /*!
     * Set audio core.
     */
    int audioInputIndex() const;
    /*!
     * Audio input index.
     */
    int audioOutputIndex() const;
    /*!
     * Audio output index.
     */

public Q_SLOTS:
    void volumeChanged(int value);
    /*!
     * Volume changed.
     */

protected:
    void initWidget();
    /*!
     * Create all widget in layout.
     */

    QComboBox *m_inputComboBox, *m_outputComboBox;
    MusicAudioRecorderCore *m_recordCore;

};

#endif // MUSICSOUNDKMICROSETTINGPOPWIDGET_H
