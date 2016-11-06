/***************************************************************************
 *   Copyright (C) 2006-2014 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
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

extern "C"
{
#include <alsa/asoundlib.h>
}

#include <QHash>
#include <qmmp/output.h>
#include <qmmp/volume.h>

class OutputALSA : public Output
{
public:
    OutputALSA();
    ~OutputALSA();

    bool initialize(quint32, ChannelMap map, Qmmp::AudioFormat format);
    //output api
    qint64 latency();
    qint64 writeAudio(unsigned char *data, qint64 maxSize);
    void drain();
    void reset();
    void suspend();
    void resume();

private:
    // helper functions
    long alsa_write(unsigned char *data, long size);
    void uninitialize();

    bool m_inited;
    bool m_use_mmap;
    //alsa
    snd_pcm_t *pcm_handle;
    char *pcm_name;
    snd_pcm_uframes_t m_chunk_size;
    //prebuffer
    uchar *m_prebuf;
    qint64 m_prebuf_size;
    qint64 m_prebuf_fill;
    bool m_can_pause;
    //channel conversions
#if (SND_LIB_VERSION >= 0x01001B)
    QHash <quint16, Qmmp::ChannelPosition> m_alsa_channels;
#endif
};

class VolumeALSA : public Volume
{
    Q_OBJECT
public:
    VolumeALSA();
    virtual ~VolumeALSA();

    void setVolume(const VolumeSettings &vol);
    VolumeSettings volume() const;

    bool hasNotifySignal() const;

private:
    //alsa mixer
    int setupMixer(QString card, QString device);
    void parseMixerName(char *str, char **name, int *index);
    int getMixer(snd_mixer_t **m_mixer, QString card);
    snd_mixer_elem_t* getMixerElem(snd_mixer_t *m_mixer, char *name, int index);
    snd_mixer_t *m_mixer;
    snd_mixer_elem_t *pcm_element;
};

#endif // OUTPUTALSA_H
