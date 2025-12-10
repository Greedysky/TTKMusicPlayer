#include "buzzichelper.h"

BuzzicHelper::BuzzicHelper(const QString &path)
    : m_path(path)
{

}

BuzzicHelper::~BuzzicHelper()
{
    deinit();
}

void BuzzicHelper::deinit()
{
    if(m_input)
    {
        Buzzic2Release(m_input);
    }
}

bool BuzzicHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("BuzzicHelper: open file failed");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    m_input = Buzzic2Load((unsigned char *)buffer.constData(), buffer.length());
    if(!m_input)
    {
        qWarning("BuzzicHelper: failed to Buzzic2Load");
        return false;
    }

    m_totalSamples = totalTime() * sampleRate() / 1000;
    return true;
}

void BuzzicHelper::seek(qint64 time)
{
    const int sample = time * sampleRate() / 1000;
    if(sample < m_currentSample)
    {
        Buzzic2Reset(m_input);
        m_currentSample = 0;
    }

    if(m_currentSample != sample)
    {
        Buzzic2Render(m_input, nullptr, sample - m_currentSample);
        m_currentSample = sample;
    }
}

qint64 BuzzicHelper::read(unsigned char *data, qint64 maxSize)
{
    if(m_currentSample >= m_totalSamples)
    {
        return 0;
    }

    const int size = maxSize / (sizeof(float) * channels());
    m_currentSample += size;
    Buzzic2Render(m_input, (StereoSample*)data, size);
    return maxSize;
}
