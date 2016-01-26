/***************************************************************************
 *   Copyright (C) 2009-2013 by Ilya Kotov                                 *
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

#ifndef REPLAYGAIN_H
#define REPLAYGAIN_H

#include <QtGlobal>
#include <QMap>
#include "decoder.h"
#include "qmmpsettings.h"
#include "qmmp.h"

/*! @internal
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class ReplayGain
{
public:
    ReplayGain();
    ~ReplayGain();

    void configure(const AudioParameters &p);
    void updateSettings(QmmpSettings::ReplayGainMode mode, double preamp,
                        double default_gain, bool clip);
    void setReplayGainInfo(const QMap<Qmmp::ReplayGainKey, double> &info, bool headroom);
    qint64 read(Decoder *decoder, char *data, qint64 size);

private:
    void updateScale();
    QMap<Qmmp::ReplayGainKey, double> m_info;
    QmmpSettings::ReplayGainMode m_mode;
    double m_scale;
    double m_preamp;
    double m_default_gain;
    float *m_prebuf;
    bool m_prevent_clipping;
    Qmmp::AudioFormat m_format;
    bool m_disabled;
    bool m_headroom;
    int m_sample_size;
};

#endif // REPLAYGAIN_H
