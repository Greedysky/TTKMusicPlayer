/***************************************************************************
 *   Copyright (C) 2006-2024 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef OUTPUTALSA_H
#define OUTPUTALSA_H

class OutputALSA;

extern "C" {
#include <alsa/asoundlib.h>
}

#include <QHash>
#include <qmmp/output.h>
#include <qmmp/volume.h>

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class OutputALSA : public Output
{
public:
    OutputALSA();
    virtual ~OutputALSA();

    virtual bool initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format) override final;

    virtual qint64 latency() override final;
    virtual qint64 writeAudio(unsigned char *data, qint64 maxSize) override final;
    virtual void drain() override final;
    virtual void reset() override final;
    virtual void suspend() override final;
    virtual void resume() override final;

private:
    // helper functions
    long alsa_write(unsigned char *data, long size);
    void uninitialize();

    bool m_inited = false;
    bool m_use_mmap;
    //alsa
    snd_pcm_t *pcm_handle = nullptr;
    char *pcm_name;
    snd_pcm_uframes_t m_chunk_size = 0;
    //prebuffer
    uchar *m_prebuf = nullptr;
    qint64 m_prebuf_size = 0;
    qint64 m_prebuf_fill = 0;
    bool m_can_pause = false;
    //channel conversions
    QHash<quint16, Qmmp::ChannelPosition> m_alsa_channels;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class VolumeALSA : public Volume
{
    Q_OBJECT
public:
    VolumeALSA();
    virtual ~VolumeALSA();

    virtual void setVolume(const VolumeSettings &vol) override final;
    virtual VolumeSettings volume() const override final;
    virtual VolumeFlags flags() const override final;

private:
    //alsa mixer
    int setupMixer(QString card, QString device);
    void parseMixerName(char *str, char **name, int *index);
    int getMixer(snd_mixer_t **m_mixer, QString card);
    snd_mixer_elem_t* getMixerElem(snd_mixer_t *m_mixer, char *name, int index);

    snd_mixer_t *m_mixer = nullptr;
    snd_mixer_elem_t *pcm_element = nullptr;

};

#endif
