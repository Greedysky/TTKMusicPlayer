/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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
 ================================================= */

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
    FFT() {}
    std::unique_ptr<FFTPlan> create(int nbits);
};

class FFTPlan
{
public:
    FFTPlan(int nbits) :
        input_size(1 << nbits), output_size((1 << (nbits - 1)) + 1),
        output(output_size)
    {
        // FFmpeg uses various assembly optimizations which expect
        // input data to be aligned by up to 32 bytes (e.g. AVX)
        this->input = (float*) av_malloc(sizeof(float) * input_size);
    }

    virtual ~FFTPlan()
    {
        av_freep(&this->input);
    }

    int get_input_size() const { return this->input_size; }
    int get_output_size() const { return this->output_size; }
    float get_input(int i) const { return this->input[i]; }
    void set_input(int i, float v) { this->input[i] = v; }
    float get_output(int i) const { return this->output[i]; }
    void set_output(int i, float v) { this->output[i] = v; }

    virtual void execute() = 0;

protected:
    float *get_input() { return this->input; }

private:
    int input_size;
    int output_size;
    float *input;
    std::vector<float> output;
};

#endif
