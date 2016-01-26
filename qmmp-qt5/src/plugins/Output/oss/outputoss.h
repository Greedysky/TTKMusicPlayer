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

#include <qmmp/output.h>
#include <qmmp/volume.h>

class OutputOSS : public Output
{
public:
    OutputOSS();
    virtual ~OutputOSS();

    bool initialize(quint32, ChannelMap map, Qmmp::AudioFormat format);
    //output api
    qint64 latency();
    qint64 writeAudio(unsigned char *data, qint64 maxSize);
    void drain();
    void reset();

private:
    //oss
    void post();
    void sync();
    QString m_audio_device;

    int m_audio_fd;
    long bl, br;
};

class VolumeOSS : public Volume
{
public:
    VolumeOSS();
    ~VolumeOSS();

    void setVolume(const VolumeSettings &vol);
    VolumeSettings volume() const;

private:
    //oss mixer
    QString m_audio_device;
    void openMixer();
    int m_mixer_fd;
    QString m_mixer_device;
    bool m_master;
};


#endif
