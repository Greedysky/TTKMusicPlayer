/***************************************************************************
 *   Copyright (C) 2009-2024 by Ilya Kotov                                 *
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

#ifndef REPLAYGAIN_H
#define REPLAYGAIN_H

#include <QMap>
#include "effect.h"
#include "qmmpsettings.h"

/*! @internal
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT ReplayGain : public Effect
{
public:
    void updateSettings(QmmpSettings::ReplayGainMode mode, double preamp, double default_gain, bool clip);
    void setReplayGainInfo(const QMap<Qmmp::ReplayGainKey, double> &info);
    virtual void applyEffect(Buffer *b) override final;

private:
    void updateScale();

    QMap<Qmmp::ReplayGainKey, double> m_info;
    QmmpSettings::ReplayGainMode m_mode = QmmpSettings::REPLAYGAIN_DISABLED;
    double m_scale = 1.0;
    double m_preamp = 0.0;
    double m_default_gain = 0.0;
    bool m_prevent_clipping = false;
    bool m_disabled = true;
    bool m_update = false;

};

#endif
