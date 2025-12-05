#include "decoder_sndfile.h"

sf_count_t sndfile_sf_vio_get_filelen(void *data)
{
    return static_cast<QIODevice *>(data)->size();
}

sf_count_t sndfile_sf_vio_seek(sf_count_t offset, int whence, void *data)
{
    QIODevice *input = static_cast<QIODevice *>(data);
    if(input->isSequential())
        return -1;

    qint64 start = 0;
    switch(whence)
    {
    case SEEK_END:
        start = input->size();
        break;
    case SEEK_CUR:
        start = input->pos();
        break;
    case SEEK_SET:
    default:
        start = 0;
    }

    if(input->seek(start + offset))
        return start + offset;
    return -1;
}

sf_count_t sndfile_sf_vio_read(void *ptr, sf_count_t count, void *data)
{
    return static_cast<QIODevice *>(data)->read((char *)ptr, count);
}

sf_count_t sndfile_sf_vio_write(const void *, sf_count_t, void *)
{
    return -1;
}

sf_count_t sndfile_sf_vio_tell(void *data)
{
    return static_cast<QIODevice *>(data)->pos();
}


DecoderSndFile::DecoderSndFile(QIODevice *input)
    : Decoder(input)
{

}

DecoderSndFile::~DecoderSndFile()
{
    deinit();
}

bool DecoderSndFile::initialize()
{
    m_bitrate = 0;
    m_totalTime = 0;
    SF_INFO snd_info;

    memset(&snd_info, 0, sizeof(snd_info));
    snd_info.format = 0;

    //setup callbacks
    m_vio.get_filelen = sndfile_sf_vio_get_filelen;
    m_vio.seek = sndfile_sf_vio_seek;
    m_vio.read = sndfile_sf_vio_read;
    m_vio.write = sndfile_sf_vio_write;
    m_vio.tell = sndfile_sf_vio_tell;

    m_sndfile = sf_open_virtual(&m_vio, SFM_READ, &snd_info, input());

    if(!m_sndfile)
    {
        qWarning("DecoderSndFile: unable to open");
        return false;
    }

    m_freq = snd_info.samplerate;
    int chan = snd_info.channels;
    m_totalTime = snd_info.frames * 1000 / m_freq;
    m_bitrate = input()->size() * 8.0 / m_totalTime + 1.0f;

    configure(m_freq, chan, Qmmp::PCM_FLOAT);
    qDebug("DecoderSndFile: detected format: %08X", snd_info.format);
    qDebug("DecoderSndFile: initialize success");
    return true;
}

void DecoderSndFile::deinit()
{
    m_totalTime = 0;
    m_bitrate = 0;
    m_freq = 0;
    if(m_sndfile)
        sf_close(m_sndfile);
    m_sndfile = nullptr;
}

qint64 DecoderSndFile::totalTime() const
{
    return m_totalTime;
}

int DecoderSndFile::bitrate() const
{
    return m_bitrate;
}

qint64 DecoderSndFile::read(unsigned char *data, qint64 maxSize)
{
    return sf_read_float(m_sndfile, (float *)data, maxSize / sizeof(float)) * sizeof(float);
}

void DecoderSndFile::seek(qint64 time)
{
    sf_seek(m_sndfile, m_freq * time / 1000, SEEK_SET);
}
