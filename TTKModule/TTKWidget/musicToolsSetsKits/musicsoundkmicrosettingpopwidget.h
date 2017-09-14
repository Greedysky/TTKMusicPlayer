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
    /*!
     * Object contsructor.
     */
    explicit MusicSoundKMicroSettingPopWidget(QWidget *parent = 0);

    virtual ~MusicSoundKMicroSettingPopWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

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
