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
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("HivelyHelper: open file failed");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    hvl_InitReplayer();
    m_input = hvl_ParseTune((unsigned char *)buffer.constData(), buffer.length(), sampleRate(), 0);
    if(!m_input)
    {
        qWarning("HivelyHelper: hvl_LoadTune error");
        return false;
    }

    m_ahxHeader = (buffer[0] == 'H') && (buffer[1] == 'V') && (buffer[2] == 'L');
    return true;
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

QString HivelyHelper::format() const
{
    return m_ahxHeader ? "AHX Tracker" : "Hively Tracker";
}
