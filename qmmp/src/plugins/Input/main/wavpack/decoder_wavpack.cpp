#include "decoder_wavpack.h"

#include <qmmp/cueparser.h>
#include <qmmp/buffer.h>

DecoderWavPack::DecoderWavPack(const QString &path)
    : Decoder(),
      m_path(path)
{

}

DecoderWavPack::~DecoderWavPack()
{
    deinit();
    delete[] m_output_buf;
    m_output_buf = nullptr;
}

bool DecoderWavPack::initialize()
{
    m_chan = 0;
    m_totalTime = 0;

    char err[80] = { 0 };
    if(m_path.startsWith("wvpack://")) //embeded cue track
    {
        const QString &filePath = TrackInfo::pathFromUrl(m_path);
#if defined(Q_OS_WIN) && defined(OPEN_FILE_UTF8)
        m_context = WavpackOpenFileInput(qUtf8Printable(filePath), err, OPEN_WVC | OPEN_TAGS | OPEN_FILE_UTF8, 0);
#else
        m_context = WavpackOpenFileInput(qPrintable(filePath), err, OPEN_WVC | OPEN_TAGS, 0);
#endif
        if(!m_context)
        {
            qWarning("DecoderWavPack: error: %s", err);
            return false;
        }
        int cue_len = WavpackGetTagItem(m_context, "cuesheet", nullptr, 0);
        if(cue_len > 0)
        {
            char *value = (char*)malloc(cue_len * 2 + 1);
            WavpackGetTagItem(m_context, "cuesheet", value, cue_len + 1);
            m_parser = new CueParser(value);
            free(value);
            m_parser->setDuration((qint64)WavpackGetNumSamples(m_context) * 1000 / WavpackGetSampleRate(m_context));
            m_parser->setUrl("wvpack", filePath);
            m_track = m_path.section("#", -1).toInt();
            if(m_track < 1 || m_track > m_parser->count())
            {
                qWarning("DecoderWavPack: invalid cuesheet comment");
                return false;
            }
            //send metadata
            QMap<Qmmp::MetaData, QString> metaData = m_parser->info(m_track)->metaData();
            addMetaData(metaData);
        }
    }
    else
#if defined(Q_OS_WIN) && defined(OPEN_FILE_UTF8)
        m_context = WavpackOpenFileInput(qUtf8Printable(m_path), err, OPEN_WVC | OPEN_TAGS | OPEN_FILE_UTF8, 0);
#else
        m_context = WavpackOpenFileInput(qPrintable(m_path), err, OPEN_WVC | OPEN_TAGS, 0);
#endif

    if(!m_context)
    {
        qWarning("DecoderWavPack: error: %s", err);
        return false;
    }

    m_chan = WavpackGetNumChannels(m_context);
    uint32_t freq = WavpackGetSampleRate(m_context);
    m_bps = WavpackGetBitsPerSample(m_context);

    ChannelMap chmap = findChannelMap(m_chan);
    if(chmap.isEmpty())
    {
        qWarning("DecoderWavPack: unsupported number of channels: %d", m_chan);
        return false;
    }

    if(!m_output_buf)
        m_output_buf = new int32_t[QMMP_BLOCK_FRAMES * m_chan];
    switch(m_bps)
    {
    case 8:
        configure(freq, chmap, Qmmp::PCM_S8);
        break;
    case 12:
    case 16:
        configure(freq, chmap, Qmmp::PCM_S16LE);
        break;
    case 20:
    case 24:
    case 32:
#ifdef MODE_FLOAT
        configure(freq, chmap, (WavpackGetMode(m_context) & MODE_FLOAT) ? Qmmp::PCM_FLOAT : Qmmp::PCM_S32LE);
#else
        configure(freq, chmap, Qmmp::PCM_S32LE);
#endif
        break;
    default:
        qWarning("DecoderWavPack: unsupported bit depth");
        return false;
    }

    if(!m_parser)
        m_totalTime = (qint64) WavpackGetNumSamples(m_context) * 1000 / freq;
    else
    {
        m_length = m_parser->duration(m_track);
        m_offset = m_parser->offset(m_track);
        m_length_in_bytes = audioParameters().sampleRate() *
                          audioParameters().frameSize() * m_length / 1000;
        setReplayGainInfo(m_parser->info(m_track)->replayGainInfo());
        seek(0);
    }

    m_totalBytes = 0;
    m_frame_size = audioParameters().frameSize();
    qDebug("DecoderWavPack: initialize success");
    return true;
}

int DecoderWavPack::bitrate() const
{
    return m_context ? int(WavpackGetInstantBitrate(m_context) / 1000) : 0;
}

qint64 DecoderWavPack::totalTime() const
{
    if(m_parser)
        return m_length;
    return m_totalTime;
}

void DecoderWavPack::deinit()
{
    m_chan = 0;
    if(m_context)
        WavpackCloseFile(m_context);
    m_context = nullptr;
    delete m_parser;
    m_parser = nullptr;
}

void DecoderWavPack::seek(qint64 time)
{
    m_totalBytes = audioParameters().sampleRate() *
                   audioParameters().channels() *
                   audioParameters().sampleSize() * time / 1000;
    if(m_parser)
        time += m_offset;
    WavpackSeekSample(m_context, time * audioParameters().sampleRate() / 1000);
}

qint64 DecoderWavPack::read(unsigned char *data, qint64 maxSize)
{
    if(m_parser)
    {
        if(m_length_in_bytes - m_totalBytes < m_frame_size) //end of cue track
            return 0;
        //returned size must contain integer number of frames
        maxSize = qMin(maxSize, (m_length_in_bytes - m_totalBytes) / m_frame_size * m_frame_size);
        maxSize = wavpack_decode(data, maxSize);
        m_totalBytes += maxSize;
        return maxSize;
    }
    return wavpack_decode(data, maxSize);
}

const QString DecoderWavPack::nextURL() const
{
    if(m_parser && m_track +1 <= m_parser->count())
        return m_parser->url(m_track + 1);
    else
        return QString();
}

void DecoderWavPack::next()
{
    if(m_parser && m_track +1 <= m_parser->count())
    {
        m_track++;
        m_offset = m_parser->duration(m_track);
        m_length = m_parser->duration(m_track);
        m_length_in_bytes = audioParameters().sampleRate() *
                          audioParameters().channels() *
                          audioParameters().sampleSize() * m_length / 1000;
        addMetaData(m_parser->info(m_track)->metaData());
        setReplayGainInfo(m_parser->info(m_track)->replayGainInfo());
        m_totalBytes = 0;
    }
}

qint64 DecoderWavPack::wavpack_decode(unsigned char *data, qint64 size)
{
    ulong len = qMin(QMMP_BLOCK_FRAMES, (int)size / m_chan / 4);
    len = WavpackUnpackSamples(m_context, m_output_buf, len);
    //convert 32 to 16
    qint8 *data8 = (qint8 *)data;
    qint16 *data16 = (qint16 *)data;
    qint32 *data32 = (qint32 *)data;
    uint i = 0;
    switch(m_bps)
    {
    case 8:
        for(i = 0;  i < len * m_chan; ++i)
            data8[i] = m_output_buf[i];
        return len * m_chan;
    case 12:
    case 16:
        for(i = 0;  i < len * m_chan; ++i)
            data16[i] = m_output_buf[i];
        return len * m_chan * 2;
    case 20:
    case 24:
        for(i = 0;  i < len * m_chan; ++i)
            data32[i] = m_output_buf[i] << 8;
        return len * m_chan * 4;
    case 32:
        for(i = 0;  i < len * m_chan; ++i)
            data32[i] = m_output_buf[i];
        return len * m_chan * 4;
    }
    return 0;
}

//http://www.wavpack.com/file_format.txt
ChannelMap DecoderWavPack::findChannelMap(int channels)
{
    ChannelMap map;
    switch(channels)
    {
    case 1:
        map << Qmmp::CHAN_FRONT_LEFT;
        break;
    case 2:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT;
        break;
    case 3:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_FRONT_CENTER;
        break;
    case 4:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT;
        break;
    case 5:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT;
        break;
    case 6:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_LFE
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT;
        break;
    case 7:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_LFE
            << Qmmp::CHAN_REAR_CENTER
            << Qmmp::CHAN_SIDE_LEFT
            << Qmmp::CHAN_SIDE_RIGHT;
        break;
    case 8:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_LFE
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT
            << Qmmp::CHAN_SIDE_LEFT
            << Qmmp::CHAN_SIDE_RIGHT;
        break;
    default:
        break;
    }
    return map;
}
