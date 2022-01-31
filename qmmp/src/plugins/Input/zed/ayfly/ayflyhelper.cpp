#include "ayflyhelper.h"

bool endCallback(void *)
{
    return true;
}

AyflyHelper::AyflyHelper(const QString &path)
    : m_path(path)
{

}

AyflyHelper::~AyflyHelper()
{
    deinit();
}

void AyflyHelper::deinit()
{
    if(m_input)
     {
        ay_closesong(&m_input);
    }
}

bool AyflyHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("AyflyHelper: open file failed");
        return false;
    }

    const qint64 size = file.size();
    const QByteArray module = file.readAll();

    if(!ay_initsongindirect((unsigned char *)module.constData(), sampleRate(), size))
    {
        qWarning("AyflyHelper: ay_initsongindirect error");
        return false;
    }

    m_input = ay_initsong(QmmpPrintable(m_path), sampleRate());
    if(!m_input)
    {
        qWarning("AyflyHelper: ay_initsong error");
        return false;
    }

    m_length = ay_getsonglength(m_input) / 50 * 1000;
    m_bitrate = size * 8.0 / totalTime() + 1.0f;
    ay_setelapsedcallback(m_input, endCallback, nullptr);

    m_metaData.insert(Qmmp::TITLE, ay_getsongname(m_input));
    m_metaData.insert(Qmmp::ARTIST, ay_getsongauthor(m_input));
    return true;
}
