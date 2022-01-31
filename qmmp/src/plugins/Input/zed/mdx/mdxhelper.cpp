#include "mdxhelper.h"

#define SAMPLE_BUF_SIZE     1024

MdxHelper::MdxHelper(const QString &path)
    : m_path(path)
{

}

MdxHelper::~MdxHelper()
{
    deinit();
}

void MdxHelper::deinit()
{
    if(m_mdx_mode)
    {
        mdx_close(&m_input);
    }
    else
    {
        pmd_stop();
    }
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
        m_mdx_mode = true;
        mdx_set_rate(sampleRate());
    }
    else
    {
        pmd_init();
        pmd_setrate(sampleRate());
    }

    char buffer[1024];
    if(m_mdx_mode)
    {
       if(mdx_open(&m_input, QmmpPrintable(m_path), nullptr) != 0)
       {
           qWarning("MdxHelper: mdx_open error");
           return false;
       }

        m_length = mdx_get_length(&m_input) * 1000;	// len in msecs: use to stop playback
        mdx_set_max_loop(&m_input, 0);
        mdx_get_title(&m_input, buffer);
        m_metaData.insert(Qmmp::TITLE, buffer);
    }
    else
    {
        if(pmd_play(QmmpPrintable(m_path), nullptr) != 0)
        {
            qWarning("MdxHelper: mdx_open error");
            return false;
        }
        m_length = pmd_length_sec() * 1000;

        pmd_get_compo(buffer);
        m_metaData.insert(Qmmp::ARTIST, buffer);

        pmd_get_title(buffer);
        m_metaData.insert(Qmmp::TITLE, buffer);
    }

    m_bitrate = size * 8.0 / totalTime() + 1.0f;
    return true;
}

qint64 MdxHelper::read(unsigned char *data, qint64)
{
    if(m_length > 0 && m_pos >= m_length)
    {
        return 0;	// stop song
    }

    if(m_mdx_mode)
    {
        mdx_calc_sample(&m_input, (short*)data, SAMPLE_BUF_SIZE);
    }
    else
    {
        pmd_renderer((short*)data, SAMPLE_BUF_SIZE);
    }

    m_pos += SAMPLE_BUF_SIZE * 1000.0 / sampleRate();
    return SAMPLE_BUF_SIZE * 4;
}
