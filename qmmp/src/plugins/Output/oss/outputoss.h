/***************************************************************************
 *   Copyright (C) 2007 by Uriy Zhuravlev stalkerg@gmail.com               *
 *                                                                         *
 *   Copyright (c) 2000-2001 Brad Hughes bhughes@trolltech.com             *
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

#ifndef OUTPUTOSS_H
#define OUTPUTOSS_H

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

/**
    @author Yuriy Zhuravlev <stalkerg@gmail.com>
*/
class OutputOSS : public Output
{
public:
    OutputOSS();
    virtual ~OutputOSS();

    virtual bool initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format) override final;

    virtual qint64 latency() override final;
    virtual qint64 writeAudio(unsigned char *data, qint64 maxSize) override final;
    virtual void drain() override final;
    virtual void reset() override final;

private:
    void post() const;
    void sync() const;

    QString m_audio_device;
    int m_audio_fd = -1;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class VolumeOSS : public Volume
{
public:
    VolumeOSS();
    virtual ~VolumeOSS();

    virtual void setVolume(const VolumeSettings &vol) override final;
    virtual VolumeSettings volume() const override final;

private:
    void openMixer();

    //oss mixer
    QString m_audio_device;
    int m_mixer_fd = -1;
    QString m_mixer_device;
    bool m_master = true;

};

#endif
