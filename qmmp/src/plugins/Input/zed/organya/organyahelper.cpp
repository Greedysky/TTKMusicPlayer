#include "organyahelper.h"

OrganyaHelper::OrganyaHelper(const QString &path)
    : m_path(path)
{
    m_info = (organya_info*)calloc(sizeof(organya_info), 1);
}

OrganyaHelper::~OrganyaHelper()
{
    deinit();
}

void OrganyaHelper::deinit()
{
    if(m_info->input)
    {
        org_decoder_destroy(m_info->input);
    }
    free(m_info);
}

bool OrganyaHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("OrganyaHelper: open file failed");
        file.close();
        return false;
    }

    const qint64 size = file.size();
    file.close();

    m_info->input = org_decoder_create(m_path.toLocal8Bit().constData(), 1);
    if(!m_info->input)
    {
        qWarning("OrganyaHelper: org_decoder_create error");
        return false;
    }

    m_info->input->state.sample_rate = sampleRate();
    m_info->bitrate = size * 8.0 / totalTime() + 1.0f;

    seek(0);

    return true;
}

int OrganyaHelper::totalTime() const
{
    return org_decoder_get_total_samples(m_info->input) / sampleRate() * 1000;
}

void OrganyaHelper::seek(qint64 time)
{
    int64_t pos = time * sampleRate() / 1000;
    org_decoder_seek_sample(m_info->input, pos);
}

int OrganyaHelper::bitrate() const
{
    return m_info->bitrate;
}

int OrganyaHelper::sampleRate() const
{
    return 44100;
}

int OrganyaHelper::channels() const
{
    return 2;
}

int OrganyaHelper::bitsPerSample() const
{
    return 2;
}

int OrganyaHelper::read(unsigned char *buf, int size)
{
    unsigned int sample = size / 2 / sizeof(int16_t);
    return org_decode_samples(m_info->input, (int16_t*)buf, sample) * 2 * sizeof(int16_t);
}
