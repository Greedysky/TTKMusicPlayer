#include "decoder_cue.h"
#include "cuefile.h"

#include <qmmp/decoderfactory.h>

DecoderCUE::DecoderCUE(const QString &path)
    : Decoder(),
      m_path(path)
{

}

DecoderCUE::~DecoderCUE()
{
    delete m_decoder;
    m_decoder = nullptr;
    delete m_cueFile;
    m_cueFile = nullptr;
    delete[] m_buf;
    m_buf = nullptr;
    if(m_input)
        m_input->deleteLater();
    m_input = nullptr;
}

bool DecoderCUE::initialize()
{
    m_cueFile = new CueFile(m_path);
    if(m_cueFile->count() == 0)
    {
        qWarning("DecoderCUE: invalid cue file");
        return false;
    }

    m_track = m_path.section("#", -1).toInt();
    m_path = m_cueFile->dataFilePath(m_track);

    if(!QFile::exists(m_path))
    {
        qWarning("DecoderCUE: file \"%s\" doesn't exist", qPrintable(m_path));
        return false;
    }

    DecoderFactory *df = Decoder::findByFilePath(m_path);
    if(!df)
    {
        qWarning("DecoderCUE: unsupported file format");
        return false;
    }

    m_length = m_cueFile->duration(m_track);
    m_offset = m_cueFile->offset(m_track);
    if(!df->properties().noInput)
    {
        m_input = new QFile(m_path);
        if(!m_input->open(QIODevice::ReadOnly))
        {
            qWarning("DecoderCUE: error: %s", qPrintable(m_input->errorString()));
            return false;
        }
    }

    m_decoder = df->create(m_path, m_input);
    if(!m_decoder->initialize())
    {
        qWarning("DecoderCUE: invalid audio file");
        return false;
    }
    m_decoder->seek(m_offset);

    configure(m_decoder->audioParameters());
    setReplayGainInfo(m_cueFile->info(m_track)->replayGainInfo());
    length_in_bytes = audioParameters().sampleRate() *
                      audioParameters().frameSize() * m_length / 1000;
    m_totalBytes = 0;
    m_sz = audioParameters().frameSize();

    addMetaData(m_cueFile->info(m_track)->metaData());
    return true;
}

qint64 DecoderCUE::totalTime() const
{
    return m_decoder ? m_length : 0;
}

void DecoderCUE::seek(qint64 time)
{
     m_decoder->seek(m_offset + time);
     m_totalBytes = audioParameters().sampleRate() *
                    audioParameters().frameSize() * time / 1000;
}

qint64 DecoderCUE::read(unsigned char *data, qint64 maxSize)
{
    if(length_in_bytes - m_totalBytes < m_sz) //end of cue track
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

    if(len + m_totalBytes <= length_in_bytes)
    {
        m_totalBytes += len;
        return len;
    }

    qint64 len2 = qMax(qint64(0), length_in_bytes - m_totalBytes);
    len2 = (len2 / m_sz) * m_sz; //whole of samples of each channel
    m_totalBytes += len2;
    //save data of the next track
    delete[] m_buf;
    m_buf_size = len - len2;
    m_buf = new char[m_buf_size];
    memmove(m_buf, data + len2, m_buf_size);
    return len2;
}

int DecoderCUE::bitrate() const
{
    return m_decoder->bitrate();
}

const QString DecoderCUE::nextURL() const
{
    if(m_track +1 <= m_cueFile->count() && m_cueFile->dataFilePath(m_track) == m_cueFile->dataFilePath(m_track + 1))
        return m_cueFile->url(m_track + 1);
    else
        return QString();
}

void DecoderCUE::next()
{
    if(m_track +1 <= m_cueFile->count())
    {
        m_track++;
        m_length = m_cueFile->duration(m_track);
        m_offset = m_cueFile->offset(m_track);
        length_in_bytes = audioParameters().sampleRate() *
                          audioParameters().frameSize() * m_length / 1000;
        addMetaData(m_cueFile->info(m_track)->metaData());
        setReplayGainInfo(m_cueFile->info(m_track)->replayGainInfo());
        m_totalBytes = 0;
    }
}
