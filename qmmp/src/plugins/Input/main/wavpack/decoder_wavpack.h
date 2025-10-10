/***************************************************************************
 *   Copyright (C) 2008-2025 by Ilya Kotov                                 *
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

#ifndef DECODER_WAVPACK_H
#define DECODER_WAVPACK_H

extern "C" {
#include <wavpack/wavpack.h>
}
#include <qmmp/decoder.h>

class CueParser;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class DecoderWavPack : public Decoder
{
public:
    explicit DecoderWavPack(const QString &);
    virtual ~DecoderWavPack();

    // Standard Decoder API
    virtual bool initialize() override final;
    virtual qint64 totalTime() const override final;
    virtual int bitrate() const override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;
    virtual void seek(qint64 time) override final;
    virtual const QString nextURL() const override final;
    virtual void next() override final;

private:
    // helper functions
    void deinit();
    qint64 wavpack_decode(unsigned char *data, qint64 size);
    ChannelMap findChannelMap(int channels);

    WavpackContext *m_context = nullptr;
    int32_t *m_output_buf = nullptr; // output buffer
    int m_chan = 0;
    qint64 m_totalTime = 0;
    qint64 m_length_in_bytes = 0;
    qint64 m_totalBytes = 0;
    qint64 m_offset = 0;
    qint64 m_length = 0;
    QString m_path;
    CueParser *m_parser = nullptr;
    int m_track = 0;
    int m_bps = 0;
    qint64 m_frame_size = 0; //frame size

};

#endif
