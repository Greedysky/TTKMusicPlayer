/***************************************************************************
 *   Copyright (C) 2010-2025 by Ilya Kotov                                 *
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

#ifndef OUTPUTOSS4_H
#define OUTPUTOSS4_H

#include <fcntl.h>
#include <unistd.h>

#include <qmmp/output.h>
#include <qmmp/volume.h>

extern "C" {
#ifdef HAVE_SYS_SOUNDCARD_H
#  include <sys/soundcard.h>
#else
#  include <soundcard.h>
#  include <sys/ioctl.h>
#endif
}

#define DEFAULT_DEV "/dev/dsp"
#define DEFAULT_MIXER "/dev/mixer"

class VolumeOSS4;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class OutputOSS4 : public Output
{
public:
    OutputOSS4();
    virtual ~OutputOSS4();

    virtual bool initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format) override final;

    virtual qint64 latency() override final;
    virtual qint64 writeAudio(unsigned char *data, qint64 maxSize) override final;
    virtual void drain() override final;
    virtual void reset() override final;

    int fd() const;

    static OutputOSS4 *instance();
    static VolumeOSS4 *m_vc;

private:
    void post() const;
    void sync() const;

    QString m_audio_device;
    int m_audio_fd = -1;
    static OutputOSS4 *m_instance;
    static Qmmp::ChannelPosition m_oss_pos[16];

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class VolumeOSS4 : public Volume
{
public:
    VolumeOSS4();
    virtual ~VolumeOSS4();

    virtual void setVolume(const VolumeSettings &vol) override final;
    virtual VolumeSettings volume() const override final;

    void restore();

private:
    int m_volume;

};

#endif
