#include "decoder_ffmpegcue.h"
#include "decoder_ffmpeg.h"

#include <qmmp/cueparser.h>

DecoderFFmpegCue::DecoderFFmpegCue(const QString &path)
    : Decoder(),
      m_path(path)
{

}

DecoderFFmpegCue::~DecoderFFmpegCue()
{
    delete m_decoder;
    m_decoder = nullptr;
    delete m_parser;
    m_parser = nullptr;
    delete[] m_buf;
    m_buf = nullptr;
    if(m_input)
        m_input->deleteLater();
    m_input = nullptr;
}

bool DecoderFFmpegCue::initialize()
{
    if(!m_path.startsWith("ffmpeg://"))
    {
        qWarning("DecoderFFmpegCue: invalid path.");
        return false;
    }

    const QString &filePath = TrackInfo::pathFromUrl(m_path, &m_track);

    AVFormatContext *in = nullptr;
#ifdef Q_OS_WIN
    if(avformat_open_input(&in, qUtf8Printable(filePath), nullptr, nullptr) < 0)
#else
    if(avformat_open_input(&in, qPrintable(filePath), nullptr, nullptr) < 0)
#endif
    {
        qDebug("DecoderFFmpegCue: unable to open file");
        return false;
    }

    avformat_find_stream_info(in, nullptr);
    AVDictionaryEntry *cuesheet = av_dict_get(in->metadata, "cuesheet", nullptr, 0);
    if(!cuesheet)
    {
        avformat_close_input(&in);
        qWarning("DecoderFFmpegCue: unable to find cuesheet comment.");
        return false;
    }

    m_parser = new CueParser(cuesheet->value);
    m_parser->setDuration(in->duration * 1000 / AV_TIME_BASE);
    m_parser->setUrl("ffmpeg", filePath);

    avformat_close_input(&in);

    if(m_track > m_parser->count() || m_parser->isEmpty())
    {
        qWarning("DecoderFFmpegCue: invalid cuesheet");
        return false;
    }

    m_input = new QFile(filePath);
    if(!m_input->open(QIODevice::ReadOnly))
    {
        qWarning("DecoderFFmpegCue: unable to open file; error: %s", qPrintable(m_input->errorString()));
        return false;
    }

    m_duration = m_parser->duration(m_track);
    m_offset = m_parser->offset(m_track);

    m_decoder = new DecoderFFmpeg(filePath, m_input);
    if(!m_decoder->initialize())
    {
        qWarning("DecoderFFapCUE: invalid audio file");
        return false;
    }
    m_decoder->seek(m_offset);

    configure(m_decoder->audioParameters());

    m_trackSize = audioParameters().sampleRate() * audioParameters().channels() *
            audioParameters().sampleSize() * m_duration / 1000;
    m_written = 0;

    m_frameSize = audioParameters().sampleSize() * audioParameters().channels();

    setReplayGainInfo(m_parser->info(m_track)->replayGainInfo()); //send ReplayGaing info
    addMetaData(m_parser->info(m_track)->metaData()); //send metadata
    return true;
}

qint64 DecoderFFmpegCue::totalTime() const
{
    return m_decoder ? m_duration : 0;
}

void DecoderFFmpegCue::seek(qint64 time)
{
    m_decoder->seek(m_offset + time);
    m_written = audioParameters().sampleRate() *
            audioParameters().channels() *
            audioParameters().sampleSize() * time / 1000;
}

qint64 DecoderFFmpegCue::read(unsigned char *data, qint64 maxSize)
{
    if(m_trackSize - m_written < m_frameSize) //end of cue track
        return 0;

    qint64 len = 0;

    if(m_buf) //read remaining data first
    {
        len = qMin(m_bufSize, maxSize);
        memmove(data, m_buf, len);
        if(maxSize >= m_bufSize)
        {
            delete[] m_buf;
            m_buf = nullptr;
            m_bufSize = 0;
        }
        else
            memmove(m_buf, m_buf + len, maxSize - len);
    }
    else
        len = m_decoder->read(data, maxSize);

    if(len <= 0) //end of file
        return 0;

    if(len + m_written <= m_trackSize)
    {
        m_written += len;
        return len;
    }

    qint64 len2 = qMax(qint64(0), m_trackSize - m_written);
    len2 = (len2 / m_frameSize) * m_frameSize; //integer number of samples
    m_written += len2;
    //save data of the next track
    delete[] m_buf;
    m_bufSize = len - len2;
    m_buf = new char[m_bufSize];
    memmove(m_buf, data + len2, m_bufSize);
    return len2;
}

int DecoderFFmpegCue::bitrate() const
{
    return m_decoder->bitrate();
}

const QString DecoderFFmpegCue::nextURL() const
{
    if(m_track +1 <= m_parser->count())
        return m_parser->url(m_track + 1);
    else
        return QString();
}

void DecoderFFmpegCue::next()
{
    if(m_track +1 <= m_parser->count())
    {
        m_track++;
        m_duration = m_parser->duration(m_track);
        m_offset = m_parser->offset(m_track);
        m_trackSize = audioParameters().sampleRate() *
                audioParameters().channels() *
                audioParameters().sampleSize() * m_duration / 1000;
        addMetaData(m_parser->info(m_track)->metaData());
        setReplayGainInfo(m_parser->info(m_track)->replayGainInfo());
        m_written = 0;
    }
}
