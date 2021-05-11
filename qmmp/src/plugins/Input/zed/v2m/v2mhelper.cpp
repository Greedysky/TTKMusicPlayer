#include "v2mhelper.h"
#include "v2mconv.h"
#include "sounddef.h"
#include <math.h>

extern "C" {
#include "stdio_file.h"
}

bool v2m_initialized = false;
int load_and_convert(const char *fname, uint8_t **conv, int *convlen)
{
    FILE *fp = stdio_open(fname);
    if(!fp)
    {
        return -1;
    }
    // probe
    const int64_t len = (int)stdio_length(fp);
    unsigned char *buf = (unsigned char *)malloc(len);
    const int rb = (int)stdio_read(buf, 1, len, fp);
    stdio_close(fp);

    if(rb != len)
    {
        free(buf);
        return -1;
    }

    if(!v2m_initialized)
    {
        sdInit();
        v2m_initialized = true;
    }

    ssbase base;
    const int ver = CheckV2MVersion(buf, len, base);
    if(ver < 0)
    {
        free(buf);
        return -1;
    }

    ConvertV2M(buf, len, conv, convlen);
    free(buf);

    return 0;
}

int get_total_samples(V2MPlayer *player)
{
    int totalsamples = 0;
    float buffer[2048 * 2];
    bool had_nonsilence = false;

    while(true)
    {
        player->Render(buffer, 2048);
        bool eof = true;
        for(int i = 0; i < 2048*2; i++)
        {
            float v = fabs(buffer[i]);
            if(v > 0.0000001f)
            {
                eof = false;
                if(totalsamples > 44100*2)
                {
                    had_nonsilence = true;
                }
                break;
            }
        }

        if(eof && had_nonsilence)
        {
            break;
        }
        totalsamples += 2048;
    }
    return totalsamples;
}


V2MHelper::V2MHelper(const QString &path)
    : m_path(path)
{
    m_info = (v2m_info_t*)calloc(sizeof(v2m_info_t), 1);
}

V2MHelper::~V2MHelper()
{
    close();
}

void V2MHelper::close()
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
    FILE *file = stdio_open(qPrintable(m_path));
    if(!file)
    {
        qWarning("V2MHelper: open file failed");
        return false;
    }

    const int64_t size = stdio_length(file);
    stdio_close(file);

    int convlen;
    if(load_and_convert(qPrintable(m_path), &m_info->tune, &convlen) < 0)
    {
        qWarning("V2MHelper: load_and_convert error");
        return false;
    }

    m_info->player = new V2MPlayer;
    m_info->player->Init();
    m_info->player->Open(m_info->tune);

    m_info->length = m_info->player->Length() * 1000;
    m_info->bitrate = size * 8.0 / m_info->length + 1.0f;

    m_info->player->Play();

    return true;
}

int V2MHelper::totalTime() const
{
    return m_info->length;
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
