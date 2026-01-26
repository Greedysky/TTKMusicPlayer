#include "decoder_ffapcue.h"
#include "decoder_ffap.h"

DecoderFFapCUE::DecoderFFapCUE(const QString &path)
    : Decoder(),
      m_path(path)
{

}

DecoderFFapCUE::~DecoderFFapCUE()
{
    delete m_decoder;
    delete m_parser;
    delete[] m_buf;
    if(m_input)
        m_input->deleteLater();
}

bool DecoderFFapCUE::initialize()
{
    if(!m_path.startsWith("ape://") || m_path.endsWith(".ape"))
    {
        qWarning("DecoderFFapCUE: invalid path.");
        return false;
    }

    const QString &filePath = TrackInfo::pathFromUrl(m_path, &m_track);

    TagLib::FileStream stream(QStringToFileName(filePath), true);
    TagLib::APE::File file(&stream);

    TagLib::APE::Tag *tag = file.APETag();

    if(!tag || !tag->itemListMap().contains("CUESHEET"))
    {
        qWarning("DecoderFFapCUE: unable to find cuesheet comment.");
        return false;
    }

    m_parser = new CueParser(tag->itemListMap()["CUESHEET"].toString().toCString(true));
    m_parser->setDuration(file.audioProperties()->lengthInMilliseconds());
    m_parser->setUrl("ape", filePath);

    if(m_track > m_parser->count() || m_parser->isEmpty())
    {
        qWarning("DecoderFFapCUE: invalid cuesheet");
        return false;
    }

    m_input = new QFile(filePath);
    if(!m_input->open(QIODevice::ReadOnly))
    {
        qWarning("DecoderFFapCUE: %s", qPrintable(m_input->errorString()));
        return false;
    }

    QMap<Qmmp::MetaData, QString> metaData = m_parser->info(m_track)->metaData();
    addMetaData(metaData); //send metadata

    m_duration = m_parser->duration(m_track);
    m_offset = m_parser->offset(m_track);

    m_decoder = new DecoderFFap(filePath, m_input);
    if(!m_decoder->initialize())
    {
        qWarning("DecoderFFapCUE: invalid audio file");
        return false;
    }
    m_decoder->seek(m_offset);

    configure(m_decoder->audioParameters().sampleRate(),
              m_decoder->audioParameters().channels(),
              m_decoder->audioParameters().format());

    m_trackSize = audioParameters().sampleRate() *
            audioParameters().channels() *
            audioParameters().sampleSize() * m_duration / 1000;
    m_written = 0;

    m_frameSize = audioParameters().sampleSize() * audioParameters().channels();

    setReplayGainInfo(m_parser->info(m_track)->replayGainInfo()); //send ReplayGaing info
    addMetaData(m_parser->info(m_track)->metaData()); //send metadata
    return true;
}

qint64 DecoderFFapCUE::totalTime() const
{
    return m_decoder ? m_duration : 0;
}

void DecoderFFapCUE::seek(qint64 time)
{
    m_decoder->seek(m_offset + time);
    m_written = audioParameters().sampleRate() * audioParameters().channels() * audioParameters().sampleSize() * time / 1000;
}

qint64 DecoderFFapCUE::read(unsigned char *data, qint64 maxSize)
{
    if(m_trackSize - m_written < m_frameSize) //end of cue track
        return 0;

    qint64 len = 0;

    if(m_buf) //read remaining data first
    {
        len = qMin(m_buf_size, maxSize);
        memmove(data, m_buf, len);
        if(maxSize >= m_buf_size)
        {
            delete[] m_buf;
            m_buf = nullptr;
            m_buf_size = 0;
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
    len2 = (len2 / m_frameSize) * m_frameSize; //whole of samples of each channel
    m_written += len2;
    //save data of the next track
    delete[] m_buf;
    m_buf_size = len - len2;
    m_buf = new char[m_buf_size];
    memmove(m_buf, data + len2, m_buf_size);
    return len2;
}

int DecoderFFapCUE::bitrate() const
{
    return m_decoder->bitrate();
}

const QString DecoderFFapCUE::nextURL() const
{
    if(m_track + 1 <= m_parser->count())
        return m_parser->url(m_track + 1);
    else
        return QString();
}

void DecoderFFapCUE::next()
{
    if(m_track + 1 <= m_parser->count())
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
