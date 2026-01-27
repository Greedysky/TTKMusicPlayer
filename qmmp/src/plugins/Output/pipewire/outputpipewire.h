/***************************************************************************
 *   Copyright (C) 2006-2025 by Ilya Kotov                                 *
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

#ifndef OUTPUTPIPEWIRE_H
#define OUTPUTPIPEWIRE_H

#include <pipewire/pipewire.h>
#include <spa/param/audio/format-utils.h>

#include <QHash>
#include <qmmp/output.h>
#include <qmmp/volume.h>

class VolumePipeWire;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class OutputPipeWire : public Output
{
public:
    OutputPipeWire();
    virtual ~OutputPipeWire();

    virtual bool initialize(quint32, ChannelMap map, Qmmp::AudioFormat format) override;

    virtual qint64 latency() override final;
    virtual qint64 writeAudio(unsigned char *data, qint64 maxSize) override final;
    virtual void drain() override final;
    virtual void reset() override final;
    virtual void suspend() override final;
    virtual void resume() override final;

    void setMuted(bool mute);
    void setVolume(const VolumeSettings &v);

    static OutputPipeWire *instance;
    static VolumePipeWire *volumeControl;

private:
    // helper functions
    void uninitialize();
    //callbacks
    static void onProcess(void *data);
    static void onDrained(void *data);
    static void onCoreEventDone(void *data, uint32_t id, int seq);
    static void onRegistryEventGlobal(void *data, uint32_t id,
                                      uint32_t permissions, const char *type,
                                      uint32_t version, const struct spa_dict *props);
    static void onStateChanged(void *data, enum pw_stream_state old,
                               enum pw_stream_state state, const char *error);

    // PipeWire objects
    struct pw_thread_loop *m_loop = nullptr;
    struct pw_stream *m_stream  = nullptr;
    pw_context *m_context = nullptr;
    pw_core *m_core = nullptr;
    pw_registry *m_registry = nullptr;
    spa_hook m_coreListener = {};
    spa_hook m_streamListener = {};
    spa_hook m_registryListener = {};

    int m_coreInitSeq = 0;

    bool m_inited = false;
    bool m_hasSinks = false;
    bool m_ignoreStateChange = false;

    unsigned char *m_buffer = nullptr;
    quint32 m_buffer_at = 0;
    quint32 m_bufferSize = 0;
    quint32 m_frames = 0;
    quint32 m_stride = 0;
    QHash <Qmmp::ChannelPosition, spa_audio_channel> m_pw_channels;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class VolumePipeWire : public Volume
{
public:
    VolumePipeWire();
    virtual ~VolumePipeWire();

    virtual void setVolume(const VolumeSettings &vol) override final;
    virtual VolumeSettings volume() const override final;
    virtual bool isMuted() const override final;
    virtual void setMuted(bool mute) override final;
    virtual VolumeFlags flags() const override final;

private:
    VolumeSettings m_volume;
    bool m_muted = false;

};

#endif
