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

#ifndef SPEKPIPELINE_H
#define SPEKPIPELINE_H

#include <memory>
#include <string>

class AudioFile;
class FFTPlan;
struct spek_pipeline;

enum WindowFunction
{
    WINDOW_HANN,
    WINDOW_HAMMING,
    WINDOW_BLACKMAN_HARRIS,
    WINDOW_COUNT,
    WINDOW_DEFAULT = WINDOW_HANN,
};

typedef void (*spek_pipeline_cb)(int bands, int sample, float *values, void *cb_data);

struct spek_pipeline * spek_pipeline_open(
    std::unique_ptr<AudioFile> file,
    std::unique_ptr<FFTPlan> fft,
    int stream,
    int channel,
    enum WindowFunction window_function,
    int samples,
    spek_pipeline_cb cb,
    void *cb_data
);

void spek_pipeline_start(struct spek_pipeline *pipeline);
void spek_pipeline_close(struct spek_pipeline *pipeline);

std::string spek_pipeline_desc(const struct spek_pipeline *pipeline);
int spek_pipeline_streams(const struct spek_pipeline *pipeline);
int spek_pipeline_channels(const struct spek_pipeline *pipeline);
double spek_pipeline_duration(const struct spek_pipeline *pipeline);
int spek_pipeline_sample_rate(const struct spek_pipeline *pipeline);

#endif
