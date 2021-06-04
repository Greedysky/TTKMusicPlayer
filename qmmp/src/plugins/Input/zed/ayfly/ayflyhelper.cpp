#include "ayflyhelper.h"

bool endCallback(void *)
{
    return true;
}

AyflyHelper::AyflyHelper(const QString &path)
    : m_path(path)
{
    m_info = (decode_info*)calloc(sizeof(decode_info), 1);
}

AyflyHelper::~AyflyHelper()
{
    deinit();
}

void AyflyHelper::deinit()
{
    if(m_info) 
    {
        if(m_info->input)
         {
            ay_closesong(&m_info->input);
        }
        free(m_info);
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

    m_info->input = ay_initsong(qPrintable(m_path), sampleRate());
    if(!m_info->input)
    {
        qWarning("AyflyHelper: ay_initsong error");
        return false;
    }

    m_info->length = ay_getsonglength(m_info->input) / 50 * 1000;
    m_info->bitrate = size * 8.0 / m_info->length + 1.0f;
    ay_setelapsedcallback(m_info->input, endCallback, nullptr);

    m_metaData.insert(Qmmp::TITLE, ay_getsongname(m_info->input));
    m_metaData.insert(Qmmp::ARTIST, ay_getsongauthor(m_info->input));
    return true;
}

int AyflyHelper::totalTime() const
{
    return m_info->length;
}

void AyflyHelper::seek(qint64 time)
{
    ay_seeksong(m_info->input, time / 1000 * 50);
}

int AyflyHelper::bitrate() const
{
    return m_info->bitrate;
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
    return ay_rendersongbuffer(m_info->input, buf, size);
}

const QMap<Qmmp::MetaData, QString> &AyflyHelper::readMetaData() const
{
    return m_metaData;
}
