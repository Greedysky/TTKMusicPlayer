#ifndef MUSICAUDIORECORDERMODULE_H
#define MUSICAUDIORECORDERMODULE_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include <QFile>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include "musicglobaldefine.h"

/*! @brief The class of the audio recorder core.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAudioRecorderModule : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAudioRecorderModule)
public:
    /*!
     * Object constructor.
     */
    explicit MusicAudioRecorderModule(QObject *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicAudioRecorderModule();

    /*!
     * Add the header of wav.
     */
    int addWavHeader(const char *fileName) const;

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
    QString fileName() const;
    /*!
     * Current input device error or not.
     */
    bool error() const;

    /*!
     * Recorder play start.
     */
    void onRecordStart();
    /*!
     * Recorder play stop.
     */
    void onRecordStop();

private:
    int m_inputVolume;
    QFile *m_file;
    QAudioFormat m_formatFile;
    QAudioInput *m_audioInputFile;
    QAudioOutput *m_audioOutputFile;

};

#endif // MUSICAUDIORECORDERMODULE_H
