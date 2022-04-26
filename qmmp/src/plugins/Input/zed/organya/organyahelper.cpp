#include "organyahelper.h"

OrganyaHelper::OrganyaHelper(const QString &path)
    : m_path(path)
{

}

OrganyaHelper::~OrganyaHelper()
{
    deinit();
}

void OrganyaHelper::deinit()
{
    if(m_input)
    {
        org_decoder_destroy(m_input);
    }
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

    m_input = org_decoder_create(QmmpPrintable(m_path), 1);
    if(!m_input)
    {
        qWarning("OrganyaHelper: org_decoder_create error");
        return false;
    }

    m_bitrate = size * 8.0 / totalTime() + 1.0f;

    seek(0);
    return true;
}

qint64 OrganyaHelper::read(unsigned char *data, qint64 maxSize)
{
    const unsigned int sample = maxSize / 2 / sizeof(int16_t);
    return org_decode_samples(m_input, (int16_t*)data, sample) * 2 * sizeof(int16_t);
}
