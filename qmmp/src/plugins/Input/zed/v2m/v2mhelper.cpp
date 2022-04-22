#include "v2mhelper.h"
#include <libv2m/v2mconv.h>
#include <libv2m/sounddef.h>

static bool v2mInitialized = false;
int loadAndConvert(unsigned char *module, qint64 size, uint8_t **conv, int *convlen)
{
    if(!v2mInitialized)
    {
        sdInit();
        v2mInitialized = true;
    }

    const int ver = CheckV2MVersion(module, size);
    if(ver < 0)
    {
        return -1;
    }

    ConvertV2M(module, size, conv, convlen);
    return 0;
}


V2MHelper::V2MHelper(const QString &path)
    : m_path(path)
{

}

V2MHelper::~V2MHelper()
{
    deinit();
}

void V2MHelper::deinit()
{
    if(m_tune)
    {
        free(m_tune);
    }

    if(m_input)
    {
        m_input->Close();
        delete m_input;
    }
}

bool V2MHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("V2MHelper: open file failed");
        return false;
    }

    const qint64 size = file.size();
    const QByteArray module = file.readAll();
    file.close();

    int convlen;
    if(loadAndConvert((unsigned char *)module.constData(), size, &m_tune, &convlen) < 0)
    {
        qWarning("V2MHelper: load_and_convert error");
        return false;
    }

    m_input = new V2MPlayer;
    m_input->Init();
    m_input->Open(m_tune);

    m_bitrate = size * 8.0 / totalTime() + 1.0f;
    m_input->Play();
    return true;
}

qint64 V2MHelper::read(unsigned char *data, qint64 maxSize)
{
    if(!m_input->IsPlaying())
    {
        return 0;
    }

    const int sampleSize = (depth() >> 3) * channels();
    const int samples = maxSize / sampleSize;

    m_input->Render((float*)data, samples);
    return maxSize;
}
