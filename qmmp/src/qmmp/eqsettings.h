/***************************************************************************
 *   Copyright (C) 2010-2024 by Ilya Kotov                                 *
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

#ifndef EQSETTINGS_H
#define EQSETTINGS_H

#include "qmmp_export.h"

/*! @brief The EqSettings class helps to work with equalizer settings.
     * @author Ilya Kotov <forkotov02@ya.ru>
     */
class QMMP_EXPORT EqSettings
{
public:
    /*!
     * Equalizer band number
     */
    enum Bands
    {
        EQ_BANDS_10 = 10, /*!< 10 bands */
        EQ_BANDS_15 = 15, /*!< 15 bands */
        EQ_BANDS_25 = 25, /*!< 25 bands */
        EQ_BANDS_31 = 31  /*!< 31 bands */
    };

    /*!
     * Constructs a copy of \b other.
     */
    EqSettings(const EqSettings &other);
    /*!
     * Constructs an empty equalizer settings (0 dB, disabled)
     * @param bands - Number of bands (supported: 10, 15, 25, 31)
     */
    explicit EqSettings(Bands bands = EQ_BANDS_10);

    /*!
     * Returns \b true if equalizer is enabled, otherwise returns \b false.
     */
    bool isEnabled() const;
    /*!
     * Returns gain of the channel number \b chan.
     */
    double gain(int chan) const;
    /*!
     * Returns equalizer preamp.
     */
    double preamp() const;
    /*!
     * Returns number of bands
     */
    int bands() const;
    /*!
     * Returns \b true if two passes is enabled, otherwise returns \b false.
     */
    bool twoPasses() const;
    /*!
     *  Enables equalizer if \p enabled is \b true or disables it if \p enabled is \b false.
     */
    void setEnabled(bool enabled = true);
    /*!
     * Sets gain of the equalizer channel.
     * @param band Number of equalizer band.
     * @param gain Channel gain (-20.0..20.0 dB)
     */
    void setGain(int band, double gain);
    /*!
     * Sets equalizer preamp to \b preamp.
     */
    void setPreamp(double preamp);
    /*!
     *  Enables two passes for equalizer if \p enabled is \b true or disables it if \p enabled is \b false.
     */
    void setTwoPasses(bool enabled = true);
    /*!
     * Assigns equalizer settings \b s to this settings.
     */
    EqSettings &operator=(const EqSettings &s);
    /*!
     * Returns \b true if equalizer settins \b s is equal to this settings; otherwise returns false.
     */
    bool operator==(const EqSettings &s) const;
    /*!
     * Returns \b true if equalizer settins \b s is not equal to this settings; otherwise returns false.
     */
    bool operator!=(const EqSettings &s) const;

private:
    double m_gains[31] = { 0 };
    double m_preamp = 0;
    bool m_is_enabled = false;
    int m_bands = 0;
    bool m_two_passes = false;

};

#endif
