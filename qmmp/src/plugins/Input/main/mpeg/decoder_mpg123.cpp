#include "decoder_mpg123.h"
#include "tagextractor.h"

#include <QVariant>

ssize_t mpg123_read_cb(void *src, void *buf, size_t size)
{
    DecoderMPG123 *d = static_cast<DecoderMPG123*>(src);
    return d->input()->read((char *)buf, size);
}

off_t mpg123_seek_cb(void *src, off_t offset, int whence)
{
    DecoderMPG123 *d = static_cast<DecoderMPG123*>(src);
    if(d->input()->isSequential())
        return -1;

    long start = 0;
    switch(whence)
    {
    case SEEK_END:
        start = d->input()->size();
        break;
    case SEEK_CUR:
        start = d->input()->pos();
        break;
    case SEEK_SET:
    default:
        start = 0;
    }

    if(!d->input()->seek(start + offset))
        return -1;
    return d->input()->pos();
}


DecoderMPG123::DecoderMPG123(QIODevice *input)
    : Decoder(input)
{

}

DecoderMPG123::~DecoderMPG123()
{
    cleanup(m_handle);
    m_handle = nullptr;
}

bool DecoderMPG123::initialize()
{
    m_errors = 0;
    if(input()->isSequential()) //for streams only
    {
        TagExtractor extractor(input());
        if(!extractor.id3v2tag().isEmpty())
            addMetaData(extractor.id3v2tag());
    }

    int err = mpg123_init();
    if(err != MPG123_OK)
    {
        qWarning("DecoderMPG123: basic setup goes wrong: %s", mpg123_plain_strerror(err));
        return false;
    }

    if(!(m_handle = mpg123_new(nullptr, &err)))
    {
        qWarning("DecoderMPG123: basic setup goes wrong: %s", mpg123_plain_strerror(err));
        return false;
    }

    mpg123_param(m_handle, MPG123_ADD_FLAGS, MPG123_SEEKBUFFER | MPG123_FUZZY, 0);

    if((err = mpg123_replace_reader_handle(m_handle, mpg123_read_cb, mpg123_seek_cb, nullptr)) != MPG123_OK)
    {
        qWarning("DecoderMPG123: mpg123 error: %s", mpg123_plain_strerror(err));
        cleanup(m_handle);
        m_handle = nullptr;
        return false;
    }
    setMPG123Format(MPG123_ENC_FLOAT_32);

    if((err = mpg123_open_handle(m_handle, this)) != MPG123_OK)
    {
        qWarning("DecoderMPG123: mpg123 error: %s", mpg123_plain_strerror(err));
        cleanup(m_handle);
        m_handle = nullptr;
        return false;
    }

    int channels = 0;
    if((err = mpg123_getformat(m_handle, &m_rate, &channels, &m_mpg123_encoding)) != MPG123_OK)
    {
        qWarning("DecoderMPG123: mpg123 error: %s", mpg123_plain_strerror(err));
        cleanup(m_handle);
        m_handle = nullptr;
        return false;
    }
    //check format
    if(m_mpg123_encoding != MPG123_ENC_FLOAT_32)
    {
        cleanup(m_handle);
        qWarning("DecoderMPG123: bad encoding: 0x%x!\n", m_mpg123_encoding);
        m_handle = nullptr;
        return false;
    }

    if(!input()->isSequential())
    {
        if((err = mpg123_scan(m_handle)) != MPG123_OK)
            qWarning("DecoderMPG123: mpg123 error: %s", mpg123_plain_strerror(err));
        //duration
        m_totalTime = (qint64) mpg123_length(m_handle) * 1000 / m_rate;
    }
    else
    {
        if((err = mpg123_info(m_handle, &m_frame_info)) != MPG123_OK)
            qWarning("DecoderMPG123: mpg123 error: %s", mpg123_plain_strerror(err));

        if(m_frame_info.version == MPG123_1_0)
            setProperty(Qmmp::FORMAT_NAME, QString("MPEG-1 layer %1").arg(m_frame_info.layer));
        else if(m_frame_info.version == MPG123_2_0)
            setProperty(Qmmp::FORMAT_NAME, QString("MPEG-2 layer %1").arg(m_frame_info.layer));
        else if(m_frame_info.version == MPG123_2_5)
            setProperty(Qmmp::FORMAT_NAME, QString("MPEG-2.5 layer %1").arg(m_frame_info.layer));

        m_totalTime = 0;
    }

    configure(m_rate, channels, Qmmp::PCM_FLOAT);
    return true;
}

qint64 DecoderMPG123::totalTime() const
{
    return m_totalTime;
}

int DecoderMPG123::bitrate() const
{
    return m_frame_info.bitrate;
}

qint64 DecoderMPG123::read(unsigned char *data, qint64 maxSize)
{
    size_t done = 0;
    int err = mpg123_read(m_handle, data, maxSize, &done);
    if(err < 0)
    {
        err = mpg123_errcode(m_handle);
        if(!m_errors)
            qWarning("DecoderMPG123: decoder error: %s", mpg123_plain_strerror(err));

        if(m_errors < 10)
        {
            m_errors++;
            if(err == MPG123_RESYNC_FAIL && done > 0)
                memset(data, 0, done);
            return done;
        }
        return -1;
    }
    else if(err != MPG123_DONE && err != MPG123_OK)
    {
        qWarning("DecoderMPG123: decoder error: %s", mpg123_plain_strerror(err));
        return -1;
    }

    m_errors = 0;
    mpg123_info(m_handle, &m_frame_info);
    return done;
}

void DecoderMPG123::seek(qint64 time)
{
    if(m_totalTime > 0)
    {
        mpg123_seek(m_handle, time * m_rate / 1000, SEEK_SET);
    }
}

void DecoderMPG123::cleanup(mpg123_handle *handle)
{
    if(handle)
    {
        mpg123_close(handle);
        mpg123_delete(handle);
    }
}

void DecoderMPG123::setMPG123Format(int encoding)
{
    int sample_rates[] = { 8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000 };

    /* Ensure that this output format will not change (it could, when we allow it). */
    mpg123_format_none(m_handle);
    for(unsigned int i = 0; i < sizeof(sample_rates)/sizeof(int); ++i)
        mpg123_format(m_handle, sample_rates[i], MPG123_MONO | MPG123_STEREO, encoding);
    m_mpg123_encoding = encoding;
}
