#include "ayflyhelper.h"

bool endCallback(void *)
{
    return true;
}

AyflyHelper::AyflyHelper(const QString &path)
    : m_path(path)
{
    m_info = (ay_info*)calloc(sizeof(ay_info), 1);
}

AyflyHelper::~AyflyHelper()
{
    deinit();
}

void AyflyHelper::deinit()
{
    if(m_info) 
    {
        if(m_info->ay)
         {
            ay_closesong(&m_info->ay);
        }
        free(m_info);
    }
}

bool AyflyHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("AsapHelper: open file failed");
        return false;
    }

    const qint64 size = file.size();
    const QByteArray module = file.readAll();

    if(!ay_initsongindirect((unsigned char *)module.constData(), sampleRate(), size))
    {
        qWarning("AyflyHelper: ay_initsongindirect error");
        return false;
    }

    m_info->ay = ay_initsong(qPrintable(m_path), sampleRate());
    if(!m_info->ay)
    {
        qWarning("AyflyHelper: ay_initsong error");
        return false;
    }

    m_info->length = ay_getsonglength(m_info->ay) / 50 * 1000;
    m_info->rate = size * 8.0 / m_info->length + 1.0f;
    ay_setelapsedcallback(m_info->ay, endCallback, nullptr);

    m_metaData.insert(Qmmp::TITLE, ay_getsongname(m_info->ay));
    m_metaData.insert(Qmmp::ARTIST, ay_getsongauthor(m_info->ay));
    return true;
}

int AyflyHelper::totalTime() const
{
    return m_info->length;
}

void AyflyHelper::seek(qint64 time)
{
    ay_seeksong(m_info->ay, time / 1000 * 50);
}

int AyflyHelper::bitrate() const
{
    return m_info->rate;
}

int AyflyHelper::sampleRate() const
{
    return 44100;
}

int AyflyHelper::channels() const
{
    return 2;
}

int AyflyHelper::bitsPerSample() const
{
    return 16;
}

int AyflyHelper::read(unsigned char *buf, int size)
{
    return ay_rendersongbuffer(m_info->ay, buf, size);
}

const QMap<Qmmp::MetaData, QString> &AyflyHelper::readMetaData() const
{
    return m_metaData;
}
