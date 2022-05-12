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
    const QByteArray &module = file.readAll();
    file.close();

    hvl_InitReplayer();
    m_input = hvl_ParseTune((unsigned char *)module.constData(), size, sampleRate(), 0);
    if(!m_input)
    {
        qWarning("HivelyHelper: hvl_LoadTune error");
        return false;
    }

    m_ahxHeader = (module[0] == 'H') && (module[1] == 'V') && (module[2] == 'L');
    return true;
}

QString HivelyHelper::format() const
{
    return m_ahxHeader ? "AHX Tracker" : "Hively Tracker";
}

QString HivelyHelper::instruments() const
{
    QString value;
    // instruments starts from 1 in hively so skip 0
    for(int i = 1; i < instrumentCount(); ++i)
    {
        value += m_input->ht_Instruments[i].ins_Name;
        value += "\n";
    }
    return value;
}

QString HivelyHelper::subSongs() const
{
    QString value;
    if(subSongCount() > 1)
    {
        for(int i = 0; i < subSongCount(); ++i)
        {
            value += m_input->ht_Name;
            value += "\n";
        }
    }
    return value;
}

qint64 HivelyHelper::read(unsigned char *data, qint64)
{
    if(m_input->ht_SongEndReached)
    {
        return 0;
    }

    int8 *ptr = (int8*)data;
    hvl_DecodeFrame(m_input, ptr, ptr + 2, 4);
    return (m_input->ht_Frequency / 50 / m_input->ht_SpeedMultiplier) * 4;
}
