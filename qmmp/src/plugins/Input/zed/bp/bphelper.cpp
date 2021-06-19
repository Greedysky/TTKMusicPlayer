#include "bphelper.h"

#include <libbp/player.h>

BpHelper::BpHelper(const QString &path)
    : m_path(path)
{
    m_info = (decode_info*)calloc(sizeof(decode_info), 1);
}

BpHelper::~BpHelper()
{
    deinit();
}

void BpHelper::deinit()
{
    if(m_info)
    {
        if(m_info->input)
        {
            delete m_info->input;
        }
        free(m_info);
    }
}

bool BpHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("BpHelper: open file failed");
        return false;
    }

    const qint64 size = file.size();
    const QByteArray module = file.readAll();

    m_info->input = new Player((BYTE*)module.constData(), size);
    if(!m_info->input->Load())
    {
        qWarning("BpHelper: Load error");
        return false;
    }

    m_info->bitrate = size * 8.0 / totalTime() + 1.0f;
    return true;
}

int BpHelper::totalTime() const
{
    return m_info->input->GetLength();
}

void BpHelper::seek(qint64 time)
{
    return m_info->input->Seek(time);
}

int BpHelper::bitrate() const
{
    return m_info->bitrate;
}

int BpHelper::sampleRate() const
{
    return 44100;
}

int BpHelper::channels() const
{
    return 2;
}

int BpHelper::bitsPerSample() const
{
    return 8;
}

int BpHelper::read(unsigned char *buf, int)
{
    if(!m_info->input->Run())
    {
        return 0;
    }
    return m_info->input->Render(buf);
}
