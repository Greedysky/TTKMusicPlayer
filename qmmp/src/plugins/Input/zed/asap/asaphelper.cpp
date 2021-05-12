#include "asaphelper.h"

AsapHelper::AsapHelper(const QString &path)
    : m_path(path)
{
    m_info = (asap_info*)calloc(sizeof(asap_info), 1);
}

AsapHelper::~AsapHelper()
{
    close();
}

void AsapHelper::close()
{
    if(m_info)
    {
        if(m_info->asap)
        {
            ASAP_Delete(m_info->asap);
        }
        free(m_info);
    }
}

bool AsapHelper::initialize()
{
    FILE *file = stdio_open(qPrintable(m_path));
    if(!file)
    {
        qWarning("AsapHelper: open file failed");
        return false;
    }

    const int64_t size = stdio_length(file);
    if(size <= 0 || size > 256 * 1024)
    {
        qWarning("AsapHelper: file size invalid");
        stdio_close(file);
        return false;
    }

    unsigned char *module = (unsigned char *)malloc(size);
    if(!module)
    {
        qWarning("AsapHelper: file data read error");
        stdio_close(file);
        return false;
    }

    stdio_read(module, size, 1, file);
    stdio_close(file);

    m_info->asap = ASAP_New();
    ASAP_DetectSilence(m_info->asap, 5);

    if(!ASAP_Load(m_info->asap, qPrintable(m_path), module, size))
    {
        qWarning("AsapHelper: ASAP_Load error");
        free(module);
        return false;
    }
    free(module);

    ASAPInfo *info =(ASAPInfo *)ASAP_GetInfo(m_info->asap);
    //struct ASAPInfo { int channels; int covoxAddr; int defaultSong; int fastplay; int headerLen;
    // int init; int music; cibool ntsc; int player; int songs; ASAPModuleType type;
    // unsigned char songPos[32]; char author[128]; char date[128]; int durations[32];
    // char filename[128]; cibool loops[32]; char title[128]; };
    if(!ASAP_PlaySong(m_info->asap, ASAPInfo_GetDefaultSong(info), 360000))
    {
        qWarning("AsapHelper: ASAP_PlaySong error");
        return false;
    }

    m_meta.insert("title", ASAPInfo_GetTitle(info));
    m_meta.insert("artist", ASAPInfo_GetAuthor(info));
    m_meta.insert("year", QString::number(ASAPInfo_GetYear(info)));

    m_info->length = ASAPInfo_GetDuration(info, ASAPInfo_GetDefaultSong(info));
    m_info->channels = ASAPInfo_GetChannels(info);
    m_info->bitrate = size * 8.0 / m_info->length + 1.0f;

    return true;
}

int AsapHelper::totalTime() const
{
    return m_info->length;
}

void AsapHelper::seek(qint64 time)
{
    ASAP_Seek(m_info->asap, time);
}

int AsapHelper::bitrate() const
{
    return m_info->bitrate;
}

int AsapHelper::sampleRate() const
{
    return ASAP_SAMPLE_RATE;
}

int AsapHelper::channels() const
{
    return m_info->channels;
}

int AsapHelper::bitsPerSample() const
{
    return 32;
}

int AsapHelper::read(unsigned char *buf, int size)
{
    if(ASAP_GetPosition(m_info->asap) >= totalTime())
    {
        return 0;
    }

    return ASAP_Generate(m_info->asap, buf, size, ASAPSampleFormat_S16_L_E);
}

QMap<QString, QString> AsapHelper::readMetaTags() const
{
    return m_meta;
}
