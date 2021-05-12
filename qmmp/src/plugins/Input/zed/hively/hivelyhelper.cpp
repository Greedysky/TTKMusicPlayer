#include "hivelyhelper.h"
#include <math.h>

extern "C" {
#include <hvl_replay.h>
#include "stdio_file.h"
}

HivelyHelper::HivelyHelper(const QString &path)
    : m_path(path)
{
    m_info = (hively_info*)calloc(sizeof(hively_info), 1);
}

HivelyHelper::~HivelyHelper()
{
    close();
}

void HivelyHelper::close()
{
    if(m_info)
    {
        if(m_info->tune)
        {
            hvl_FreeTune(m_info->tune);
        }

        free(m_info);
    }
}

bool HivelyHelper::initialize()
{
    FILE *file = stdio_open(qPrintable(m_path));
    if(!file)
    {
        qWarning("HivelyHelper: open file failed");
        return false;
    }

    const int64_t size = stdio_length(file);
    if(size <= 0 || size > 256 * 1024)
    {
        qWarning("HivelyHelper: file size invalid");
        stdio_close(file);
        return false;
    }

    unsigned char *module = (unsigned char *)malloc(size);
    if(!module)
    {
        qWarning("HivelyHelper: file data read error");
        stdio_close(file);
        return false;
    }

    stdio_read(module, size, 1, file);
    stdio_close(file);

    hvl_InitReplayer();

    m_info->tune = hvl_ParseTune(module, size, sampleRate(), 0);
    if(!m_info->tune)
    {
        qDebug("HivelyHelper: hvl_LoadTune error");
        free(module);
        return false;
    }
    free(module);

    m_info->bitrate = size * 8.0 / totalTime() + 1.0f;

    bool ahx = true;
    if((module[0] == 'H') && (module[1] == 'V') && (module[2] == 'L'))
    {
        ahx = false;
    }

    const char* tool = ahx ? "AHX Tracker" : "Hively Tracker";
    m_meta.insert("SongTypeTag", tool);
    m_meta.insert("AuthoringToolTag", tool);
    m_meta.insert("TitleTag", m_info->tune->ht_Name);
    
    QString instruments;
    // instruments starts from 1 in hively so skip 0
    for(int i = 1; i < m_info->tune->ht_InstrumentNr; ++i)
    {
        instruments += m_info->tune->ht_Instruments[i].ins_Name;
        instruments += " ";
    }
    m_meta.insert("Instruments", instruments);

    QString subsongs;
    if(m_info->tune->ht_SubsongNr > 1)
    {
        for(int i = 0, c = m_info->tune->ht_SubsongNr; i < c; ++i)
        {
            char subsong_name[1024] = {0};
            sprintf(subsong_name, "%s (%d/%d)", m_info->tune->ht_Name, i + 1, m_info->tune->ht_SubsongNr);

            subsongs += subsong_name;
            subsongs += " ";
        }
    }
    m_meta.insert("SubSongs", subsongs);

    return true;
}

int HivelyHelper::totalTime() const
{
    return hvl_GetPlayTime(m_info->tune);
}

void HivelyHelper::seek(qint64 time)
{
    hvl_Seek(m_info->tune, time);
}

int HivelyHelper::bitrate() const
{
    return m_info->bitrate;
}

int HivelyHelper::sampleRate() const
{
    return 44100;
}

int HivelyHelper::channels() const
{
    return 2;
}

int HivelyHelper::bitsPerSample() const
{
    return 32;
}

int HivelyHelper::read(unsigned char *buffer, int )
{
    if(m_info->tune->ht_SongEndReached)
    {
        return 0;
    }

    int8* ptr = (int8*)buffer;
    hvl_DecodeFrame(m_info->tune, ptr, ptr + 2, 4);

    const int sample = (m_info->tune->ht_Frequency / 50 / m_info->tune->ht_SpeedMultiplier) * 4;
    return sample;
}
