#include "hivelyhelper.h"
#include <math.h>

extern "C" {
#include <libhively/hvl_replay.h>
}

HivelyHelper::HivelyHelper(const QString &path)
    : m_path(path)
{
    m_info = (decode_info*)calloc(sizeof(decode_info), 1);
}

HivelyHelper::~HivelyHelper()
{
    deinit();
}

void HivelyHelper::deinit()
{
    if(m_info)
    {
        if(m_info->input)
        {
            hvl_FreeTune(m_info->input);
        }

        free(m_info);
    }
}

bool HivelyHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("HivelyHelper: open file failed");
        return false;
    }

    const qint64 size = file.size();
    const QByteArray module = file.readAll();

    hvl_InitReplayer();
    m_info->input = hvl_ParseTune((unsigned char *)module.constData(), size, sampleRate(), 0);
    if(!m_info->input)
    {
        qDebug("HivelyHelper: hvl_LoadTune error");
        return false;
    }

    m_info->bitrate = size * 8.0 / totalTime() + 1.0f;

    bool ahx = true;
    if((module[0] == 'H') && (module[1] == 'V') && (module[2] == 'L'))
    {
        ahx = false;
    }

    const char* tool = ahx ? "AHX Tracker" : "Hively Tracker";
    m_metaData.insert(Qmmp::ALBUM/*"SongTypeTag"*/, tool);
    m_metaData.insert(Qmmp::ARTIST/*"AuthoringToolTag"*/, tool);
    m_metaData.insert(Qmmp::TITLE/*"TitleTag"*/, m_info->input->ht_Name);
    
    QString instruments;
    // instruments starts from 1 in hively so skip 0
    for(int i = 1; i < m_info->input->ht_InstrumentNr; ++i)
    {
        instruments += m_info->input->ht_Instruments[i].ins_Name;
        instruments += " ";
    }
    m_metaData.insert(Qmmp::COMMENT/*"Instruments"*/, instruments);

    QString subsongs;
    if(m_info->input->ht_SubsongNr > 1)
    {
        for(int i = 0, c = m_info->input->ht_SubsongNr; i < c; ++i)
        {
            char subsong_name[1024] = {0};
            sprintf(subsong_name, "%s (%d/%d)", m_info->input->ht_Name, i + 1, m_info->input->ht_SubsongNr);

            subsongs += subsong_name;
            subsongs += " ";
        }
    }
    m_metaData.insert(Qmmp::TRACK/*"SubSongs"*/, subsongs);

    return true;
}

int HivelyHelper::totalTime() const
{
    return hvl_GetPlayTime(m_info->input);
}

void HivelyHelper::seek(qint64 time)
{
    hvl_Seek(m_info->input, time);
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
    return 16;
}

int HivelyHelper::read(unsigned char *buffer, int )
{
    if(m_info->input->ht_SongEndReached)
    {
        return 0;
    }

    int8* ptr = (int8*)buffer;
    hvl_DecodeFrame(m_info->input, ptr, ptr + 2, 4);

    const int sample = (m_info->input->ht_Frequency / 50 / m_info->input->ht_SpeedMultiplier) * 4;
    return sample;
}

const QMap<Qmmp::MetaData, QString> &HivelyHelper::readMetaData() const
{
    return m_metaData;
}
