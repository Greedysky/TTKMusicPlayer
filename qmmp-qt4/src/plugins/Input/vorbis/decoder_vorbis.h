// Copyright (c) 2000-2001 Brad Hughes <bhughes@trolltech.com>
//
// Use, modification and distribution is allowed without limitation,
// warranty, or liability of any kind.
//

#ifndef DECODER_VORBIS_H
#define DECODER_VORBIS_H

#include <qmmp/decoder.h>

#include <vorbis/vorbisfile.h>


class DecoderVorbis : public Decoder
{
public:
    DecoderVorbis(const QString &url, QIODevice *i);
    virtual ~DecoderVorbis();

    // Standard Decoder API
    bool initialize();
    qint64 totalTime();
    int bitrate();

private:
    virtual qint64 read(char *data, qint64 maxSize);
    virtual qint64 read(float *data, qint64 samples);
    virtual void seek(qint64 time);

    // helper functions
    void deinit();
    void updateTags();
    ChannelMap findChannelMap(int channels);

    // OggVorbis Decoder
    OggVorbis_File oggfile;
    qint64 m_totalTime;
    long len;
    int m_section, m_last_section;
    int m_bitrate;
    bool inited;
    QString m_url;
};


#endif // __decoder_vorbis_h
