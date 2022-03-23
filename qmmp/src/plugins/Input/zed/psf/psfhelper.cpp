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
}

bool PSFHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("PSFHelper: open file failed");
        return false;
    }

    m_file_size = file.size();
    const QByteArray module = file.readAll();

    m_type = ao_identify((char *)module.constData());
    if(m_type < 0)
    {
        qWarning("PSFHelper: ao_identify error");
        return false;
    }

    m_input = ao_start(m_type, QmmpPrintable(m_path), (uint8 *)module.constData(), m_file_size);
    if(!m_input)
    {
        qWarning("PSFHelper: ao_start error");
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
        if(!strncasecmp(info.title[i], "Length: ", 8))
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
            break;
        }
    }
    return true;
}

void PSFHelper::seek(qint64 time)
{
    const int sample = time * sampleRate() / 1000;
    if(sample > m_current_sample)
    {
        m_samples_to_skip = sample - m_current_sample;
    }
    else
    {
        ao_command(m_type, m_input, COMMAND_RESTART, 0);
        m_samples_to_skip = sample;
    }

    m_current_sample = sample;
}

qint64 PSFHelper::read(unsigned char *data, qint64 maxSize)
{
    if(m_current_sample >= m_length * sampleRate())
    {
        return 0;
    }

    const int initSize = maxSize;
    while(maxSize > 0)
    {
        if(m_remaining > 0)
        {
            if(m_samples_to_skip > 0)
            {
                int n = std::min<int>(m_samples_to_skip, m_remaining);
                if(m_remaining > n)
                {
                    memmove(m_buffer, m_buffer + n * 4, (m_remaining - n) * 4);
                }
                m_remaining -= n;
                m_samples_to_skip -= n;
                continue;
            }

            int n = maxSize / 4;
            n = std::min<int>(m_remaining, n);
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

    m_current_sample += (initSize - maxSize) / (channels() * depth() / 8);
    return initSize - maxSize;
}

QMap<Qmmp::MetaData, QString> PSFHelper::readMetaData() const
{
    QMap<Qmmp::MetaData, QString> metaData;
    if(m_type < 0 || !m_input)
    {
        return metaData;
    }

    ao_display_info info;
    memset(&info, 0, sizeof(info));
    bool have_info = false;
    if(ao_get_info(m_type, m_input, &info) == AO_SUCCESS)
    {
       have_info = true;
    }

    if(have_info)
    {
        return metaData;
    }

    for(int i = 1; i < 9; ++i)
    {
        if(!strncasecmp(info.title[i], "Name: ", 6) || !strncasecmp(info.title[i], "Song: ", 6))
        {
            metaData.insert(Qmmp::TITLE, info.info[i]);
        }
        else if(!strncasecmp(info.title[i], "Game: ", 6))
        {
            metaData.insert(Qmmp::ALBUM, info.info[i]);
        }
        else if(!strncasecmp(info.title[i], "Artist: ", 8))
        {
            metaData.insert(Qmmp::ARTIST, info.info[i]);
        }
        else if(!strncasecmp(info.title[i], "Copyright: ", 11))
        {
            metaData.insert(Qmmp::COMPOSER, info.info[i]);
        }
        else if(!strncasecmp(info.title[i], "Year: ", 6))
        {
            metaData.insert(Qmmp::YEAR, info.info[i]);
        }
        else if(!strncasecmp(info.title[i], "Fade: ", 6))
        {
            // do nothing
        }
        else
        {
            char *colon = strchr(info.title[i], ':');
            char name[colon - info.title[i] + 1];
            memcpy(name, info.title[i], colon - info.title[i]);
            name[colon - info.title[i]] = 0;
            metaData.insert(Qmmp::COMMENT, info.info[i]);
        }
    }
    return metaData;
}
