#include "v2mhelper.h"
#include "v2mconv.h"
#include "sounddef.h"
#include <math.h>

static bool v2m_initialized = false;
int load_and_convert(unsigned char *module, qint64 size, uint8_t **conv, int *convlen)
{
    if(!v2m_initialized)
    {
        sdInit();
        v2m_initialized = true;
    }

    ssbase base;
    const int ver = CheckV2MVersion(module, size, base);
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
    m_info = (v2m_info*)calloc(sizeof(v2m_info), 1);
}

V2MHelper::~V2MHelper()
{
    deinit();
}

void V2MHelper::deinit()
{
    if(m_info) 
    {
        if(m_info->tune)
        {
            free(m_info->tune);
        }

        if(m_info->player)
        {
            m_info->player->Close();
            delete m_info->player;
        }
        free(m_info);
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

    int convlen;
    if(load_and_convert((unsigned char *)module.constData(), size, &m_info->tune, &convlen) < 0)
    {
        qWarning("V2MHelper: load_and_convert error");
        return false;
    }

    m_info->player = new V2MPlayer;
    m_info->player->Init();
    m_info->player->Open(m_info->tune);

    m_info->bitrate = size * 8.0 / totalTime() + 1.0f;
    m_info->player->Play();

    return true;
}

int V2MHelper::totalTime() const
{
    return m_info->player->Length() * 1000;
}

void V2MHelper::seek(qint64 time)
{
    m_info->player->Play(time);
}

int V2MHelper::bitrate() const
{
    return m_info->bitrate;
}

int V2MHelper::sampleRate() const
{
    return 44100;
}

int V2MHelper::channels() const
{
    return 2;
}

int V2MHelper::bitsPerSample() const
{
    return 32;
}

int V2MHelper::read(unsigned char *buf, int size)
{
    if(!m_info->player->IsPlaying())
    {
        return 0;
    }

    const int samplesize = (bitsPerSample() >> 3) * channels();
    const int samples = size / samplesize;

    m_info->player->Render((float*)buf, samples);

    return size;
}
