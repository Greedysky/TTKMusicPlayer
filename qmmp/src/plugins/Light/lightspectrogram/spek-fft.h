/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2025 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef SPEKFFT_H
#define SPEKFFT_H

#include <memory>
#include <vector>

extern "C" {
#include <libavutil/mem.h>
}

class FFTPlan;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class FFT
{
public:
    FFT()
    {

    }

    std::unique_ptr<FFTPlan> create(int nbits);

};

class FFTPlan
{
public:
    FFTPlan(int nbits) :
        m_input_size(1 << nbits), m_output_size((1 << (nbits - 1)) + 1),
        m_output(m_output_size)
    {
        // FFmpeg uses various assembly optimizations which expect
        // input data to be aligned by up to 32 bytes (e.g. AVX)
        m_input = (float*) av_malloc(sizeof(float) * m_input_size);
    }

    virtual ~FFTPlan()
    {
        av_freep(&m_input);
    }

    int get_input_size() const { return m_input_size; }
    int get_output_size() const { return m_output_size; }
    float get_input(int i) const { return m_input[i]; }
    void set_input(int i, float v) { m_input[i] = v; }
    float get_output(int i) const { return m_output[i]; }
    void set_output(int i, float v) { m_output[i] = v; }

    virtual void execute() = 0;

protected:
    float *get_input() { return m_input; }

private:
    int m_input_size;
    int m_output_size;
    float *m_input;
    std::vector<float> m_output;

};

#endif
