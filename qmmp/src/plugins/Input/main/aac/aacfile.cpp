#include "aacfile.h"

#include <QBuffer>
#include <neaacdec.h>

#define MAX_CHANNELS 6
#define AAC_BUFFER_SIZE 4096

static int adts_sample_rates[] = {96000,88200,64000,48000,44100,32000,24000,22050,16000,12000,11025,8000,7350,0,0,0};

AACFile::AACFile(QIODevice *input, bool metaData, bool adts)
    : m_input(input)
{
    char buf[AAC_BUFFER_SIZE];
    qint64 buf_at = input->peek((char *) buf, AAC_BUFFER_SIZE);

    //read id3v2 tag if possible
    if(!memcmp(buf, "ID3", 3))
    {
        TagLib::ByteVector byteVector(buf, sizeof(buf));
        TagLib::ID3v2::Header header(byteVector);

        if(input->isSequential())
        {
            if(header.tagSize() >= buf_at)
            {
                qWarning("AACFile: unable to parse id3v2");
                return;
            }

            if(metaData)
                parseID3v2(QByteArray(buf, header.tagSize()));

            buf_at = sizeof(buf) - header.tagSize();
            memmove(buf, buf + header.tagSize() + header.tagSize(), sizeof(buf) - header.tagSize());
        }
        else
        {
            if(metaData)
            {
                if(header.tagSize() <= buf_at)
                    parseID3v2(QByteArray(buf, header.tagSize()));
                else
                    parseID3v2(input->read(header.tagSize()));
            }

            input->seek(header.tagSize());
            buf_at = input->read(buf, sizeof(buf));
            input->seek(0); //restore inital position
        }

        m_offset += header.tagSize();
    }

    int adts_offset = 0;

    while(adts_offset < buf_at - 6)
    {
        int adts_pos = adts_offset;
        int adts_frames = 0;

        while(adts_pos < buf_at - 6)
        {
            //try to determnate header type;
            if((uchar) buf[adts_pos] == 0xff && (((uchar)buf[adts_pos + 1] & 0xf6) == 0xf0))
            {
                unsigned int frame_length = uchar(buf[adts_pos + 3]) << 11;
                frame_length |= uchar(buf[adts_pos + 4]) << 3;
                frame_length |= uchar(buf[adts_pos + 5]) >> 5;
                frame_length &= 0x1FFF;

                if(frame_length < 7)
                {
                    adts_frames = 0;
                    break;
                }

                adts_pos += frame_length;
                adts_frames++;
                continue;
            }

            adts_frames = 0;
            break;
        }

        if(adts_frames > 1)
        {
            qDebug("AACFile: ADTS header found");
            if(!input->isSequential() && adts)
                parseADTS();

            m_isValid = true;
            m_offset += adts_offset;
            return;
        }

        adts_offset++;
    }

    if(memcmp(buf, "ADIF", 4) == 0)
    {
        qDebug("AACFile: ADIF header found");
        const int skip_size = (buf[4] & 0x80) ? 9 : 0;
        m_bitrate = ((buf[4 + skip_size] & 0x0F) << 19) |
                (buf[5 + skip_size] << 11) |
                (buf[6 + skip_size] << 3) |
                (buf[7 + skip_size] & 0xE0);

        if(!input->isSequential())
            m_duration = (qint64)(((float)input->size() * 8000.f) / ((float)m_bitrate) + 0.5f);
        else
            m_duration = 0;

        m_bitrate = (int)((float)m_bitrate / 1000.0f + 0.5f);
        m_isValid = true;
    }
}

qint64 AACFile::duration() const
{
    return m_duration;
}

quint32 AACFile::bitrate() const
{
    return m_bitrate;
}

quint32 AACFile::sampleRate() const
{
    return m_sampleRate;
}

int AACFile::offset() const
{
    return m_offset;
}

bool AACFile::isValid() const
{
    return m_isValid;
}

const QMap<Qmmp::MetaData, QString> &AACFile::metaData() const
{
    return m_metaData;
}

void AACFile::parseADTS()
{
    uchar buf[FAAD_MIN_STREAMSIZE * MAX_CHANNELS];
    qint64 buf_at = 0;
    int frames, frame_length;
    int t_framelength = 0;
    float frames_per_sec, bytes_per_frame;
    qint64 pos = m_input->pos();

    m_input->seek(0);
    buf_at = m_input->read((char *)buf, FAAD_MIN_STREAMSIZE * MAX_CHANNELS);

    for(int i = 0; i < buf_at - 1; ++i)
    {
        if(buf[i] == 0xff && (buf[i + 1] & 0xf6) == 0xf0)
        {
            memmove(buf, buf + i, buf_at - i);
            buf_at -= i;
            break;
        }
    }

    /* Read all frames to ensure correct time and bitrate */
    for(frames = 0; /* */; ++frames)
    {
        buf_at += m_input->read((char *)buf + buf_at, FAAD_MIN_STREAMSIZE * MAX_CHANNELS - buf_at);

        if(buf_at > 7)
        {
            /* check syncword */
            if(!((buf[0] == 0xFF)&&((buf[1] & 0xF6) == 0xF0)))
                break;

            if(frames == 0)
                m_sampleRate = adts_sample_rates[(buf[2] & 0x3c)>>2];

            frame_length = ((((unsigned int)buf[3] & 0x3)) << 11)
                           | (((unsigned int)buf[4]) << 3) | (buf[5] >> 5);

            if(frame_length == 0)
                break;

            t_framelength += frame_length;

            if(frame_length > buf_at)
                break;

            buf_at -= frame_length;
            memmove(buf, buf + frame_length, buf_at);
        }
        else
        {
            break;
        }
    }
    m_input->seek(pos);
    frames_per_sec = (float) m_sampleRate / 1024.0f;
    if(frames != 0)
        bytes_per_frame = (float)t_framelength/(float)(frames * 1000);
    else
        bytes_per_frame = 0;
    m_bitrate = (quint32)(8. * bytes_per_frame * frames_per_sec + 1.0f);

    if(frames_per_sec != 0)
        m_duration = frames * 1000 / frames_per_sec;
    else
        m_duration = 1000;

     m_input->seek(0); //restore inital position
}

void AACFile::parseID3v2(const QByteArray &data)
{
    ID3v2Tag taglib_tag(data);
    if(taglib_tag.isEmpty())
    {
        return;
    }

    TagLib::String album = taglib_tag.album();
    TagLib::String artist = taglib_tag.artist();
    TagLib::String comment = taglib_tag.comment();
    TagLib::String genre = taglib_tag.genre();
    TagLib::String title = taglib_tag.title();

    m_metaData.insert(Qmmp::ALBUM, TStringToQString(album).trimmed());
    m_metaData.insert(Qmmp::ARTIST, TStringToQString(artist).trimmed());
    m_metaData.insert(Qmmp::COMMENT, TStringToQString(comment).trimmed());
    m_metaData.insert(Qmmp::GENRE, TStringToQString(genre).trimmed());
    m_metaData.insert(Qmmp::TITLE, TStringToQString(title).trimmed());
    m_metaData.insert(Qmmp::YEAR, QString::number(taglib_tag.year()));
    m_metaData.insert(Qmmp::TRACK, QString::number(taglib_tag.track()));
}

ID3v2Tag::ID3v2Tag(const QByteArray &array)
    : TagLib::ID3v2::Tag(),
      m_buf(array)
{
    read();
}

void ID3v2Tag::read()
{
    if(TagLib::ID3v2::Header::size() > (uint)m_buf.length())
        return;

    header()->setData(TagLib::ByteVector(m_buf.constData(), TagLib::ID3v2::Header::size()));

    if(header()->tagSize() > (uint)m_buf.length())
        return;

    TagLib::ByteVector v(m_buf.constData() + header()->size(), header()->tagSize());
    parse(v);
}
