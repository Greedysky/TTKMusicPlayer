#include "hivelyhelper.h"

HivelyHelper::HivelyHelper(const QString &path)
    : m_path(path)
{

}

HivelyHelper::~HivelyHelper()
{
    deinit();
}

void HivelyHelper::deinit()
{
    if(m_input)
    {
        hvl_FreeTune(m_input);
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
    m_input = hvl_ParseTune((unsigned char *)module.constData(), size, sampleRate(), 0);
    if(!m_input)
    {
        qDebug("HivelyHelper: hvl_LoadTune error");
        return false;
    }

    m_bitrate = size * 8.0 / totalTime() + 1.0f;

    bool ahx = true;
    if((module[0] == 'H') && (module[1] == 'V') && (module[2] == 'L'))
    {
        ahx = false;
    }

    const char* tool = ahx ? "AHX Tracker" : "Hively Tracker";
    m_metaData.insert(Qmmp::ALBUM/*"SongTypeTag"*/, tool);
    m_metaData.insert(Qmmp::ARTIST/*"AuthoringToolTag"*/, tool);
    m_metaData.insert(Qmmp::TITLE/*"TitleTag"*/, m_input->ht_Name);
    
    QString instruments;
    // instruments starts from 1 in hively so skip 0
    for(int i = 1; i < m_input->ht_InstrumentNr; ++i)
    {
        instruments += m_input->ht_Instruments[i].ins_Name;
        instruments += " ";
    }
    m_metaData.insert(Qmmp::COMMENT/*"Instruments"*/, instruments);

    QString subsongs;
    if(m_input->ht_SubsongNr > 1)
    {
        for(int i = 0, c = m_input->ht_SubsongNr; i < c; ++i)
        {
            char subsong_name[1024] = {0};
            sprintf(subsong_name, "%s (%d/%d)", m_input->ht_Name, i + 1, m_input->ht_SubsongNr);

            subsongs += subsong_name;
            subsongs += " ";
        }
    }

    m_metaData.insert(Qmmp::TRACK/*"SubSongs"*/, subsongs);
    return true;
}

qint64 HivelyHelper::read(unsigned char *data, qint64)
{
    if(m_input->ht_SongEndReached)
    {
        return 0;
    }

    int8* ptr = (int8*)data;
    hvl_DecodeFrame(m_input, ptr, ptr + 2, 4);
    return (m_input->ht_Frequency / 50 / m_input->ht_SpeedMultiplier) * 4;
}
