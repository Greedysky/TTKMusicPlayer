/***************************************************************************
 *   Copyright (C) 2010-2014 by Ilya Kotov                                 *
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

#ifndef OUTPUTOSS4_H
#define OUTPUTOSS4_H

#include <qmmp/output.h>
#include <qmmp/volume.h>

#define DEFAULT_DEV "/dev/dsp"
#define DEFAULT_MIXER "/dev/mixer"

class VolumeOSS4;

/**
    @author Ilya Kotov <forkotov@hotmail.ru>
*/
class OutputOSS4 : public Output
{
public:
    OutputOSS4();
    virtual ~OutputOSS4();

    bool initialize(quint32, ChannelMap map, Qmmp::AudioFormat format);
    int fd();

    //output api
    qint64 writeAudio(unsigned char *data, qint64 maxSize);
    void drain();
    void reset();
    qint64 latency();

    static OutputOSS4 *instance();
    static VolumeOSS4 *m_vc;

private:
    void post();
    void sync();
    QString m_audio_device;
    int m_audio_fd;
    static OutputOSS4 *m_instance;
    static Qmmp::ChannelPosition m_oss_pos[16];

};

class VolumeOSS4 : public Volume
{
public:
    VolumeOSS4();
    ~VolumeOSS4();

    void setVolume(const VolumeSettings &vol);
    VolumeSettings volume() const;
    void restore();

private:
    int m_volume;
};

#endif
