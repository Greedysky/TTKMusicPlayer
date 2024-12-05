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

#ifndef SPEKAUDIO_H
#define SPEKAUDIO_H

#include <memory>
#include <ostream>
#include <string>

class AudioFile;
enum class AudioError;

/*!
 * @author Greedysky <greedysky@163.com>
 */
class Audio
{
public:
    Audio();
    ~Audio();

    std::unique_ptr<AudioFile> open(const std::string& file_name, int stream);

};

/*!
 * @author Greedysky <greedysky@163.com>
 */
class AudioFile
{
public:
    virtual ~AudioFile()
    {

    }

    virtual void start(int channel, int samples) = 0;
    virtual int read() = 0;

    virtual AudioError get_error() const = 0;
    virtual std::string get_codec_name() const = 0;
    virtual int get_bit_rate() const = 0;
    virtual int get_sample_rate() const = 0;
    virtual int get_bits_per_sample() const = 0;
    virtual int get_streams() const = 0;
    virtual int get_channels() const = 0;
    virtual double get_duration() const = 0;
    virtual const float *get_buffer() const = 0;
    virtual int64_t get_frames_per_interval() const = 0;
    virtual int64_t get_error_per_interval() const = 0;
    virtual int64_t get_error_base() const = 0;

};

enum class AudioError
{
    OK,
    CANNOT_OPEN_FILE,
    NO_STREAMS,
    NO_AUDIO,
    NO_DECODER,
    NO_DURATION,
    NO_CHANNELS,
    CANNOT_OPEN_DECODER,
    BAD_SAMPLE_FORMAT,
};

inline bool operator!(AudioError error)
{
    return error == AudioError::OK;
}

inline std::ostream& operator<<(std::ostream& os, AudioError error)
{
    return os << static_cast<int>(error);
}

#endif
