#include "decoder_wavpack.h"

#include <qmmp/buffer.h>
#include <qmmp/cueparser.h>

// WavPack callbacks
static int32_t wv_read_bytes(void *id, void *data, int32_t bcount)
{
    QIODevice *d = static_cast<QIODevice*>(id);
    return d->read(static_cast<char*>(data), bcount);
}

static int64_t wv_get_pos(void *id)
{
    QIODevice *d = static_cast<QIODevice*>(id);
    return d->pos();
}

static int wv_set_pos_rel(void *id, int64_t delta, int mode)
{
    QIODevice *d = static_cast<QIODevice*>(id);
    switch(mode)
    {
    case SEEK_SET:
        return d->seek(delta) ? 0 : -1;
    case SEEK_CUR:
        return d->seek(d->pos() + delta) ? 0 : -1;
    case SEEK_END:
        return d->seek(d->size() + delta) ? 0 : -1;
    default:
        break;
    }
    return -1;
}

static int wv_set_pos_abs(void *id, int64_t pos)
{
    return wv_set_pos_rel(id, pos, SEEK_SET);
}

static int wv_push_back_byte(void *id, int c)
{
    QIODevice *d = static_cast<QIODevice*>(id);
    if(d->pos() == 0)
        return EOF;

    d->ungetChar(c);
    return c;
}

static int64_t wv_get_length(void *id)
{
    QIODevice *d = static_cast<QIODevice*>(id);
    return d->size();
}

static int wv_can_seek(void *id)
{
    QIODevice *d = static_cast<QIODevice*>(id);
    return !d->isSequential();
}

//callbacks
WavpackStreamReader64 DecoderWavPack::m_reader = {
    .read_bytes = wv_read_bytes,
    .write_bytes = nullptr,
    .get_pos = wv_get_pos,
    .set_pos_abs = wv_set_pos_abs,
    .set_pos_rel = wv_set_pos_rel,
    .push_back_byte = wv_push_back_byte,
    .get_length = wv_get_length,
    .can_seek = wv_can_seek,
    .truncate_here = nullptr,
    .close = nullptr
};


DecoderWavPack::DecoderWavPack(const QString &path, QIODevice *input)
    : Decoder(input),
      m_path(path),
      m_wv_input(input)
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

    if(!m_wv_input)
    {
        if(m_path.startsWith("wvpack://")) //embeded cue track
        {
            const QString &filePath = TrackInfo::pathFromUrl(m_path);
            m_wv_input = new QFile(filePath);
            if(!m_wv_input->open(QIODevice::ReadOnly))
            {
                qWarning("DecoderWavPack: unable to open input file. Error: %s", qPrintable(m_wv_input->errorString()));
                return false;
            }

            openCorrectionFile(filePath);
            if(!(m_context = WavpackOpenFileInputEx64(&m_reader, m_wv_input, m_wvc_input, err, OPEN_WVC | OPEN_TAGS, 0)))
            {
                qWarning("DecoderWavPack: WavpackOpenFileInputEx64 error: %s", err);
                return false;
            }

            const int cue_len = WavpackGetTagItem(m_context, "cuesheet", nullptr, 0);
            if(cue_len > 0)
            {
                char *value = (char*)malloc(cue_len * 2 + 1);
                WavpackGetTagItem(m_context, "cuesheet", value, cue_len + 1);
                m_parser = new CueParser(value);
                free(value);
                m_parser->setDuration(WavpackGetNumSamples64(m_context) * 1000 / WavpackGetSampleRate(m_context));
                m_parser->setUrl("wvpack", filePath);
                m_track = m_path.section("#", -1).toInt();
                if(m_track < 1 || m_track > m_parser->count())
                {
                    qWarning("DecoderWavPack: invalid cuesheet comment");
                    return false;
                }

                m_path = filePath;
                //send metadata
                QMap<Qmmp::MetaData, QString> metaData = m_parser->info(m_track)->metaData();
                addMetaData(metaData);
            }
            else
            {
                qWarning("DecoderWavPack: missing cuesheet comment");
                return false;
            }
        }
        else
        {
            qWarning("DecoderWavPack: can not initialize. No input");
            return false;
        }
    }
    else
    {
        openCorrectionFile(m_path);
        if(!(m_context = WavpackOpenFileInputEx64(&m_reader, m_wv_input, m_wvc_input, err, OPEN_WVC | OPEN_TAGS, 0)))
        {
            qWarning("DecoderWavPack: WavpackOpenFileInputEx64 error: %s", err);
            return false;
        }
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
        configure(freq, chmap, (WavpackGetMode(m_context) & MODE_FLOAT) ? Qmmp::PCM_FLOAT : Qmmp::PCM_S32LE);
        break;
    default:
        qWarning("DecoderWavPack: unsupported bit depth");
        return false;
    }

    if(!m_parser)
        m_totalTime = WavpackGetNumSamples64(m_context) * 1000 / freq;
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
    setProperty(Qmmp::FORMAT_NAME, "WavPack");
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
    delete m_wvc_input;
    m_wvc_input = nullptr;
    if(!input() && m_wv_input)
    {
        delete m_wv_input;
        m_wv_input = nullptr;
    }
}

void DecoderWavPack::seek(qint64 time)
{
    m_totalBytes = audioParameters().sampleRate() *
                   audioParameters().channels() *
                   audioParameters().sampleSize() * time / 1000;
    if(m_parser)
        time += m_offset;
    WavpackSeekSample64(m_context, time * audioParameters().sampleRate() / 1000);
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
    if(m_parser && m_track + 1 <= m_parser->count())
        return m_parser->url(m_track + 1);
    return QString();
}

void DecoderWavPack::next()
{
    if(m_parser && m_track + 1 <= m_parser->count())
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

void DecoderWavPack::openCorrectionFile(const QString &path)
{
    const QString &wvcFilePath = path + QLatin1Char('c');
    if(QFile::exists(wvcFilePath))
    {
        m_wvc_input = new QFile(wvcFilePath);
        if(!m_wvc_input->open(QIODevice::ReadOnly))
        {
            qWarning("DecoderWavPack: Unable to open \"correction\" file. Error: %s", qPrintable(m_wvc_input->errorString()));
            delete m_wvc_input;
            m_wvc_input = nullptr;
        }
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
