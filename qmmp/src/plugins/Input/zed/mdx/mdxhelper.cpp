#include "mdxhelper.h"

#define SAMPLE_BUF_SIZE     1024

MDXHelper::MDXHelper(const QString &path)
    : m_path(path)
{
    memset(&m_mdx, 0, sizeof(MDXMini));
}

MDXHelper::~MDXHelper()
{
    deinit();
}

void MDXHelper::deinit()
{
    if(!m_mdx.self)
    {
        return;
    }

    switch(m_type)
    {
    case MDX:
        mdx_close(&m_mdx);
        break;
    case PMD:
        pmd_stop();
        break;
    default: break;
    }
}

bool MDXHelper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("MDXHelper: open file failed");
        file.close();
        return false;
    }

    const qint64 size = file.size();

    char buffer[1024];
    const QString &suffix = m_path.toLower();

    if(suffix.endsWith(".mdx"))
    {
        m_type = MDX;
    }
    else if(suffix.endsWith(".m"))
    {
        m_type = PMD;
    }
    else if(suffix.endsWith(".mub") || suffix.endsWith(".muc"))
    {
        m_type = MUC;
    }

    switch(m_type)
    {
    case MDX:
    {
        mdx_set_rate(sampleRate());

        if(mdx_open(&m_mdx, QmmpPrintable(m_path), nullptr) != 0)
        {
            qWarning("MDXHelper: mdx_open error");
            return false;
        }

        m_length = mdx_get_length(&m_mdx) * 1000;

        mdx_set_max_loop(&m_mdx, 0);
        mdx_get_title(&m_mdx, buffer);
        m_metaData.insert(Qmmp::TITLE, buffer);
        break;
    }
    case PMD:
    {
        pmd_init();
        pmd_setrate(sampleRate());

        if(pmd_play(QmmpPrintable(m_path), nullptr) != 0)
        {
            qWarning("MDXHelper: mdx_open error");
            return false;
        }

        m_length = pmd_length_sec() * 1000;

        pmd_get_compo(buffer);
        m_metaData.insert(Qmmp::ARTIST, buffer);
        pmd_get_title(buffer);
        m_metaData.insert(Qmmp::TITLE, buffer);
        break;
    }
    case MUC:
    {
        m_muc.SetRate(sampleRate());
        const QByteArray &module = file.readAll();
        m_muc.OpenMemory((unsigned char *)module.constData(), size, suffix.endsWith(".mub"));
        m_muc.UseFader(true);
        m_muc.Play();

        m_length = m_muc.GetLength() * 1000;
        m_metaData.insert(Qmmp::TITLE, QString::fromStdString(m_muc.tag->title));
        m_metaData.insert(Qmmp::ARTIST, QString::fromStdString(m_muc.tag->author));
        m_metaData.insert(Qmmp::COMPOSER, QString::fromStdString(m_muc.tag->composer));
        break;
    }
    default: break;
    }

    file.close();
    m_bitrate = size * 8.0 / totalTime() + 1.0f;
    return true;
}

qint64 MDXHelper::read(unsigned char *data, qint64)
{
    if(m_type != MUC)
    {
        if(m_length > 0 && m_pos >= m_length)
        {
            return 0;	// stop song
        }

        if(m_type == MDX)
        {
            mdx_calc_sample(&m_mdx, (short*)data, SAMPLE_BUF_SIZE);
        }
        else
        {
            pmd_renderer((short*)data, SAMPLE_BUF_SIZE);
        }

        m_pos += SAMPLE_BUF_SIZE * 1000.0 / sampleRate();
    }
    else
    {
        if(m_length > 0 && m_muc.IsEnd())
        {
            return 0;	// stop song
        }

        m_muc.Mix((short*)data, SAMPLE_BUF_SIZE);
    }
    return SAMPLE_BUF_SIZE * 4;
}
