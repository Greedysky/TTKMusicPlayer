#include "qoahelper.h"

QOAHelper::QOAHelper(const QString &path)
    : m_path(path)
{

}

QOAHelper::~QOAHelper()
{
    deinit();
}

void QOAHelper::deinit()
{
    if(m_input)
    {
        qoaplay_close(m_input);
    }
}

bool QOAHelper::initialize()
{
    m_input = qoaplay_open(qPrintable(m_path));
    if(!m_input)
    {
        qWarning("QOAHelper: failed to qoaplay_open");
        return false;
    }
    return true;
}

void QOAHelper::seek(qint64 time)
{
    qoaplay_seek_frame(m_input, (time * sampleRate() / 1000) / QOA_FRAME_LEN);
}

qint64 QOAHelper::read(unsigned char *data, qint64 maxSize)
{
    if(m_input->sample_pos >= m_input->info.samples)
    {
        return -1;
    }

    const int size = sizeof(float) * channels();
    return qoaplay_decode(m_input, (float*)data, maxSize / size) * size;
}
