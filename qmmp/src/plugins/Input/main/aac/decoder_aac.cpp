#include "decoder_aac.h"
#include "aacfile.h"

#include <QVariant>

#define AAC_BUFFER_SIZE 4096

DecoderAAC::DecoderAAC(QIODevice *input)
    : Decoder(input)
{

}

DecoderAAC::~DecoderAAC()
{
    if(data())
    {
        if(data()->handle)
            NeAACDecClose(data()->handle);
        delete data();
        m_data = nullptr;
    }

    delete[] m_input_buf;
    m_input_buf = nullptr;
    m_bitrate = 0;
}

bool DecoderAAC::initialize()
{
    m_bitrate = 0;
    m_totalTime = 0;

    if(!input())
    {
        qWarning("DecoderAAC: cannot initialize.  No input.");
        return false;
    }

    if(!m_input_buf)
        m_input_buf = new char[AAC_BUFFER_SIZE];
    m_input_at = 0;

    AACFile aac_file(input());
    if(!aac_file.isValid())
    {
        qWarning("DecoderAAC: unsupported AAC file");
        return false;
    }

    //skip id3 tag and partial frame
    if(aac_file.offset() > 0)
    {
        qDebug("DecoderAAC: header offset = %d bytes", aac_file.offset());

        char *data = new char[aac_file.offset()];
        input()->read(data, aac_file.offset());
        delete[] data;
    }

    m_totalTime = aac_file.duration();
    m_bitrate = aac_file.bitrate();

    if(!m_data)
        m_data = new aac_data;

    data()->handle = NeAACDecOpen();

    NeAACDecConfigurationPtr conf;
    conf = NeAACDecGetCurrentConfiguration(data()->handle);
    conf->downMatrix = 1;
    conf->defSampleRate = 44100;
    conf->dontUpSampleImplicitSBR = 0;
    conf->defObjectType = LC;
    conf->outputFormat = FAAD_FMT_16BIT;
    NeAACDecSetConfiguration(data()->handle, conf);

    m_input_at = input()->read((char *)m_input_buf, AAC_BUFFER_SIZE);

    unsigned long freq = 0;
    unsigned char chan = 0;
    int res = NeAACDecInit(data()->handle, (unsigned char*) m_input_buf, m_input_at, &freq, &chan);
    if(res < 0)
    {
        qWarning("DecoderAAC: NeAACDecInit() failed");
        return false;
    }

    if(!freq || !chan)
    {
        qWarning("DecoderAAC: invalid sound parameters");
        return false;
    }

    memmove(m_input_buf, m_input_buf + res, m_input_at - res);
    m_input_at -= res;

    setProperty(Qmmp::FORMAT_NAME, "AAC");
    configure(freq, chan, Qmmp::PCM_S16LE);
    qDebug("DecoderAAC: initialize success");
    return true;
}

qint64 DecoderAAC::read(unsigned char *data, qint64 maxSize)
{
    NeAACDecFrameInfo frame_info;
    qint64 size = 0, to_read, read;
    bool eof = false;

    while(m_sample_buf_size <= 0 && !eof)
    {
        m_sample_buf_at = 0;
        if(m_input_at < AAC_BUFFER_SIZE)
        {
            to_read = AAC_BUFFER_SIZE - m_input_at;
            read = input()->read((char *) (m_input_buf + m_input_at),  to_read);
            eof = (read != to_read);
            m_input_at += read;
        }

        if(m_input_at == 0)
            return 0;

        m_sample_buf = NeAACDecDecode(this->data()->handle, &frame_info, (uchar *)m_input_buf, m_input_at);
        memmove(m_input_buf, m_input_buf + frame_info.bytesconsumed,
                m_input_at - frame_info.bytesconsumed);

        m_input_at -= frame_info.bytesconsumed;

        if(eof && frame_info.error > 0) //ignore error on end of file
            return 0;
        else if(frame_info.error > 0)
        {
            m_input_at = 0;
            qDebug("DecoderAAC: %s", NeAACDecGetErrorMessage(frame_info.error));
            return -1;
        }
        if(frame_info.samples > 0)
            m_bitrate = frame_info.bytesconsumed * 8 * frame_info.samplerate * frame_info.channels
                    / frame_info.samples / 1000;

        m_sample_buf_size = frame_info.samples * 2;
    }

    if(m_sample_buf_size > 0)
    {
        size = qMin(m_sample_buf_size, maxSize);
        memcpy(data, (char *)(m_sample_buf) + m_sample_buf_at, size);
        m_sample_buf_at += size;
        m_sample_buf_size -= size;
    }
    return size;
}

qint64 DecoderAAC::totalTime() const
{
    return m_totalTime;
}

int DecoderAAC::bitrate() const
{
    return m_bitrate;
}

void DecoderAAC::seek(qint64 time)
{
    input()->seek(time * input()->size() / m_totalTime);
    NeAACDecPostSeekReset(data()->handle, 0);
    m_input_at = 0;
}
