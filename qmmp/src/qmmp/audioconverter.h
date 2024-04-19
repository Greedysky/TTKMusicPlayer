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

#ifndef AUDIOCONVERTER_H
#define AUDIOCONVERTER_H

#include <stddef.h>
#include "qmmp.h"

/*! @brief The AbstractEngine class provides the internal audio converter.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT AudioConverter
{
public:
    /*!
     * Object constructor.
     */
    AudioConverter() = default;

    /*!
     * Sets working audio format.
     * This function should be called before object usage.
     * \param f Audio format.
     */
    void configure(Qmmp::AudioFormat f);
    /*!
     * Converts samples from specified working format to \b Qmmp::PCM_FLOAT format.
     * \param in Input buffer.
     * \param out Output buffer.
     * \param samples Number of samples.
     */
    void toFloat(const unsigned char *in, float *out, size_t samples);
    /*!
     * Converts samples from \b Qmmp::PCM_FLOAT format to specified working format.
     * \param in Input buffer.
     * \param out Output buffer.
     * \param samples Number of samples.
     */
    void fromFloat(const float *in, unsigned char *out, size_t samples);

private:
    Qmmp::AudioFormat m_format = Qmmp::PCM_UNKNOWN;

};

#endif
