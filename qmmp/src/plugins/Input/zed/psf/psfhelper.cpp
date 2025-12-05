#include "psfhelper.h"

extern "C" {
#include <libpsf/ao.h>
}

PSFHelper::PSFHelper(const QString &path)
    : m_path(path)
{

}

PSFHelper::~PSFHelper()
{
    deinit();
}

void PSFHelper::deinit()
{
    if(m_input && m_type >= 0)
    {
        ao_stop(m_type, m_input);
    }
    delete[] m_data;
}

bool PSFHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("PSFHelper: open file failed");
        return false;
    }

    const qint64 size = file.size();
    m_data = new char[size]{0};
    file.read(m_data, size);
    file.close();

    m_type = ao_identify(m_data);
    if(m_type < 0)
    {
        qWarning("PSFHelper: ao_identify error");
        return false;
    }

    m_input = ao_start(m_type, QmmpPrintable(m_path), (uint8 *)m_data, size);
    if(!m_input)
    {
        qWarning("PSFHelper: unable to open file");
        return false;
    }

    ao_display_info info;
    memset(&info, 0, sizeof(info));
    bool have_info = false;
    if(ao_get_info(m_type, m_input, &info) == AO_SUCCESS)
    {
       have_info = true;
    }

    m_length = 120;

    if(!have_info)
    {
        qDebug("PSFHelper: ao has no display info");
        return true;
    }

    for(int i = 1; i < 9; ++i)
    {
        if(!strncasecmp(info.title[i], "Length:", 7))
        {
            int min;
            float sec;
            if(sscanf(info.info[i], "%d:%f", &min, &sec) == 2)
            {
                m_length = min * 60 + sec;
            }
            else if(sscanf(info.info[i], "%f", &sec) == 1)
            {
                m_length = sec;
            }
        }
        else if(!strncasecmp(info.title[i], "Name:", 5) || !strncasecmp(info.title[i], "Song:", 5))
        {
            m_metaData.insert("title", info.info[i]);
        }
        else if(!strncasecmp(info.title[i], "Artist:", 7))
        {
            m_metaData.insert("artist", info.info[i]);
        }
        else if(!strncasecmp(info.title[i], "Year:", 5))
        {
            m_metaData.insert("year", info.info[i]);
        }
        else if(!strncasecmp(info.title[i], "Game:", 5))
        {
            m_metaData.insert("game", info.info[i]);
        }
        else if(!strncasecmp(info.title[i], "Fade:", 5))
        {
            m_metaData.insert("fade", info.info[i]);
        }
        else if(!strncasecmp(info.title[i], "Ripper:", 7))
        {
            m_metaData.insert("ripper", info.info[i]);
        }
        else if(!strncasecmp(info.title[i], "Copyright:", 10))
        {
            m_metaData.insert("copyright", info.info[i]);
        }
        else
        {
            const char *colon = strchr(info.title[i], ':');
            if(colon)
            {
                char name[colon - info.title[i] + 1];
                memcpy(name, info.title[i], colon - info.title[i]);
                name[colon - info.title[i]] = 0;
                m_metaData.insert("title", info.info[i]);
            }
        }
    }
    return true;
}

void PSFHelper::seek(qint64 time)
{
    const int sample = time * sampleRate() / 1000;
    if(sample > m_currentSample)
    {
        m_samplesToSkip = sample - m_currentSample;
    }
    else
    {
        ao_command(m_type, m_input, COMMAND_RESTART, 0);
        m_samplesToSkip = sample;
    }

    m_currentSample = sample;
}

qint64 PSFHelper::read(unsigned char *data, qint64 maxSize)
{
    if(m_currentSample >= m_length * sampleRate())
    {
        return 0;
    }

    const int initSize = maxSize;
    while(maxSize > 0)
    {
        if(m_remaining > 0)
        {
            if(m_samplesToSkip > 0)
            {
                const int n = std::min<int>(m_samplesToSkip, m_remaining);
                if(m_remaining > n)
                {
                    memmove(m_buffer, m_buffer + n * 4, (m_remaining - n) * 4);
                }

                m_remaining -= n;
                m_samplesToSkip -= n;
                continue;
            }

            const int n = std::min<int>(m_remaining, maxSize / 4);
            memcpy(data, m_buffer, n * 4);
            if(m_remaining > n)
            {
                memmove(m_buffer, m_buffer + n * 4, (m_remaining - n) * 4);
            }

            m_remaining -= n;
            data += n * 4;
            maxSize -= n * 4;
        }

        if(!m_remaining)
        {
            ao_decode(m_type, m_input, (int16_t *)m_buffer, 735);
            m_remaining = 735;
        }
    }

    m_currentSample += (initSize - maxSize) / (channels() * depth() / 8);
    return initSize - maxSize;
}
