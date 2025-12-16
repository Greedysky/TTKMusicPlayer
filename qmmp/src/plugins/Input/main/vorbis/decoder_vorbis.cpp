#include "decoder_vorbis.h"

#include <qmmp/output.h>

static size_t oggread(void *buf, size_t size, size_t nmemb, void *src)
{
    if(! src) return 0;

    DecoderVorbis *dogg = static_cast<DecoderVorbis*>(src);
    int len = dogg->input()->read((char *) buf, (size * nmemb));
    return len / size;
}

static int oggseek(void *src, ogg_int64_t offset, int whence)
{
    DecoderVorbis *dogg = static_cast<DecoderVorbis*>(src);

    if(dogg->input()->isSequential())
        return -1;

    long start = 0;
    switch(whence)
    {
    case SEEK_END:
        start = dogg->input()->size();
        break;

    case SEEK_CUR:
        start = dogg->input()->pos();
        break;

    case SEEK_SET:
    default:
        start = 0;
    }

    if(dogg->input()->seek(start + offset))
        return 0;
    return -1;
}

static int oggclose(void *)
{
    return 0;
}

static long oggtell(void *src)
{
    DecoderVorbis *dogg = (DecoderVorbis *) src;
    long t = dogg->input()->pos();
    return t;
}


DecoderVorbis::DecoderVorbis(QIODevice *input)
    : Decoder(input)
{
    memset(&oggfile, 0, sizeof(OggVorbis_File));
}

DecoderVorbis::~DecoderVorbis()
{
    deinit();
}

bool DecoderVorbis::initialize()
{
    m_inited = false;
    m_totalTime = 0;

    if(!input())
    {
        qWarning("DecoderVorbis: cannot initialize.  No input");
        return false;
    }

    ov_callbacks oggcb =
    {
        oggread,
        oggseek,
        oggclose,
        oggtell
    };

    if(ov_open_callbacks(this, &oggfile, nullptr, 0, oggcb) < 0)
    {
        qWarning("DecoderVorbis: cannot open stream");
        return false;
    }

    quint32 freq = 0;
    m_bitrate = ov_bitrate(&oggfile, -1) / 1000;
    int chan = 0;

    if((m_totalTime = ov_time_total(&oggfile, -1) * 1000) < 0)
        m_totalTime = 0;

    vorbis_info *ogginfo = ov_info(&oggfile, -1);
    if(ogginfo)
    {
        freq = ogginfo->rate;
        chan = ogginfo->channels;
        setProperty(Qmmp::BITRATE, int(ogginfo->bitrate_nominal / 1000));
        setProperty(Qmmp::FORMAT_NAME, "Ogg Vorbis");
    }

    ChannelMap chmap = findChannelMap(chan);
    if(chmap.isEmpty())
    {
        qWarning("DecoderVorbis: unsupported number of channels: %d", chan);
        return false;
    }

    configure(freq, chmap, Qmmp::PCM_FLOAT);
    m_inited = true;
    qDebug("DecoderVorbis: initialize success");
    return true;
}

qint64 DecoderVorbis::totalTime() const
{
    if(!m_inited)
        return 0;
    return m_totalTime;
}

int DecoderVorbis::bitrate() const
{
    return m_bitrate;
}

void DecoderVorbis::deinit()
{
    if(m_inited)
        ov_clear(&oggfile);
    len = 0;
}

void DecoderVorbis::updateTags()
{
    vorbis_comment *comments;
    QMap<Qmmp::MetaData, QString> metaData;
    comments = ov_comment(&oggfile, -1);

    for(int i = 0; i < comments->comments; ++i)
    {
        if(!strncasecmp(comments->user_comments[i], "title=", strlen("title=")))
            metaData.insert(Qmmp::TITLE, QString::fromUtf8(comments->user_comments[i] + strlen("title=")));
        else if(!strncasecmp(comments->user_comments[i], "artist=", strlen("artist=")))
            metaData.insert(Qmmp::ARTIST, QString::fromUtf8(comments->user_comments[i] + strlen("artist=")));
        else if(!strncasecmp(comments->user_comments[i], "album=", strlen("album=")))
            metaData.insert(Qmmp::ALBUM, QString::fromUtf8(comments->user_comments[i] + strlen("album=")));
        else if(!strncasecmp(comments->user_comments[i], "comment=", strlen("comment=")))
            metaData.insert(Qmmp::COMMENT, QString::fromUtf8(comments->user_comments[i] + strlen("comment=")));
        else if(!strncasecmp(comments->user_comments[i], "genre=", strlen("genre=")))
            metaData.insert(Qmmp::GENRE, QString::fromUtf8(comments->user_comments[i] + strlen("genre=")));
        else if(!strncasecmp(comments->user_comments[i], "tracknumber=", strlen("tracknumber=")))
            metaData.insert(Qmmp::TRACK, QString::number(atoi(comments->user_comments[i] + strlen("tracknumber="))));
        else if(!strncasecmp(comments->user_comments[i], "track=", strlen("track=")))
            metaData.insert(Qmmp::TRACK, QString::number(atoi(comments->user_comments[i] + strlen("track="))));
        else if(!strncasecmp(comments->user_comments[i], "date=", strlen("date=")))
            metaData.insert(Qmmp::YEAR, QString::number(atoi(comments->user_comments[i] + strlen("date="))));
        else if(!strncasecmp(comments->user_comments[i], "composer=", strlen("composer=")))
            metaData.insert(Qmmp::COMPOSER, QString::fromUtf8(comments->user_comments[i] + strlen("composer=")));
        else if(!strncasecmp(comments->user_comments[i], "discnumber=", strlen("discnumber=")))
            metaData.insert(Qmmp::DISCNUMBER, QString::number(atoi(comments->user_comments[i] + strlen("discnumber="))));
    }
    addMetaData(metaData);
}

//http://xiph.org/vorbis/doc/Vorbis_I_spec.html#x1-800004.3.9
ChannelMap DecoderVorbis::findChannelMap(int channels)
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
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_FRONT_RIGHT;
        break;
    case 4:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT;
        break;
    case 5:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT;
        break;
    case 6:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT
            << Qmmp::CHAN_LFE;
        break;
    case 7:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_SIDE_LEFT
            << Qmmp::CHAN_SIDE_RIGHT
            << Qmmp::CHAN_REAR_CENTER
            << Qmmp::CHAN_LFE;
        break;
    case 8:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_SIDE_LEFT
            << Qmmp::CHAN_SIDE_RIGHT
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT
            << Qmmp::CHAN_LFE;
        break;
    default:
        break;
    }
    return map;
}

void DecoderVorbis::seek(qint64 time)
{
    ov_time_seek(&oggfile, (double) time / 1000);
}

qint64 DecoderVorbis::read(unsigned char *data, qint64 maxSize)
{
    len = -1;
    float **pcm = nullptr;
    int section = 0;
    while(len < 0)
        len = ov_read_float(&oggfile, &pcm, maxSize/sizeof(float), &section);

    if(len == 0)
        return 0;

    int channels = audioParameters().channels();

    for(int i = 0; i < channels; ++i)
    {
        float *ptr = (float *) (data + i*sizeof(float));
        for(int j = 0; j < len; ++j)
        {
            *ptr = pcm[i][j];
            ptr += channels;
        }
    }

    if(section != m_last_section)
    {
        updateTags();
        m_last_section = section;
    }

    m_bitrate = ov_bitrate_instant(&oggfile) / 1000;
    return len*sizeof(float)*channels;
}
