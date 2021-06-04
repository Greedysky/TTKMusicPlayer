#include "mdxhelper.h"

#define SAMPLE_BUF_SIZE     1024

MdxHelper::MdxHelper(const QString &path)
    : m_path(path)
{
    m_info = (decode_info*)calloc(sizeof(decode_info), 1);
}

MdxHelper::~MdxHelper()
{
    deinit();
}

void MdxHelper::deinit()
{
    if(m_info) 
    {
        if(m_info->mdx_mode)
        {
            mdx_close(&m_info->input);
        }
        else
        {
            pmd_stop();
        }
    }
    free(m_info);
}

bool MdxHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("MdxHelper: open file failed");
        file.close();
        return false;
    }

    const qint64 size = file.size();
    file.close();

    if(m_path.toLower().endsWith(".mdx"))
    {
        m_info->mdx_mode = true;
        mdx_set_rate(sampleRate());
    }
    else
    {
        pmd_init();
        pmd_setrate(sampleRate());
    }

    char buffer[1024];
    if(m_info->mdx_mode)
    {
       if(mdx_open(&m_info->input, qPrintable(m_path), nullptr) != 0)
       {
           qWarning("MdxHelper: mdx_open error");
           return false;
       }

        m_info->length = mdx_get_length(&m_info->input) * 1000;	// len in msecs: use to stop playback
        mdx_set_max_loop(&m_info->input, 0);
        mdx_get_title(&m_info->input, buffer);
        m_metaData.insert(Qmmp::TITLE, buffer);
    }
    else
    {
        if(pmd_play(qPrintable(m_path), nullptr) != 0)
        {
            qWarning("MdxHelper: mdx_open error");
            return false;
        }
        m_info->length = pmd_length_sec() * 1000;

        pmd_get_compo(buffer);
        m_metaData.insert(Qmmp::ARTIST, buffer);

        pmd_get_title(buffer);
        m_metaData.insert(Qmmp::TITLE, buffer);
    }

    m_info->bitrate = size * 8.0 / m_info->length + 1.0f;
    return true;
}

int MdxHelper::totalTime() const
{
    return m_info->length;
}

void MdxHelper::seek(qint64 time)
{

}

int MdxHelper::bitrate() const
{
    return m_info->bitrate;
}

int MdxHelper::sampleRate() const
{
    return 44100;
}

int MdxHelper::channels() const
{
    return 2;
}

int MdxHelper::bitsPerSample() const
{
    return 2;
}

int MdxHelper::read(unsigned char *buf, int)
{
    if(m_info->length > 0 && m_info->pos >= m_info->length)
    {
        return 0;	// stop song
    }

    if(m_info->mdx_mode)
    {
        mdx_calc_sample(&m_info->input, (short*)buf, SAMPLE_BUF_SIZE);
    }
    else
    {
        pmd_renderer((short*)buf, SAMPLE_BUF_SIZE);
    }

    m_info->pos += SAMPLE_BUF_SIZE * 1000.0 / sampleRate();
    return SAMPLE_BUF_SIZE * 4;
}

const QMap<Qmmp::MetaData, QString> &MdxHelper::readMetaData() const
{
    return m_metaData;
}
