#include "adplughelper.h"

#include <QFileInfo>

AdplugHelper::AdplugHelper(const QString &path)
    : m_filePath(path)
{
    m_opl = new CEmuopl(rate(), true, false);
    m_player = CAdPlug::factory(path.toStdString(), m_opl);
}

AdplugHelper::~AdplugHelper()
{
    delete m_opl;
    delete m_player;
}

AdplugHelper::Frame AdplugHelper::read()
{
    size_t to_write;
    size_t bufsiz = sizeof(m_buf) / sizeof(*m_buf);

    if(m_remaining == 0)
    {
        if(!m_player->update())
        {
            return Frame(0, nullptr);
        }
        m_remaining = rate() / m_player->getrefresh();
    }

    if(m_remaining > bufsiz)
    {
        to_write = bufsiz;
    }
    else
    {
        to_write = m_remaining;
    }

    m_opl->update(m_buf, to_write);
    m_remaining -= to_write;
    return Frame(to_write * 2, reinterpret_cast<unsigned char *>(m_buf));
}

bool AdplugHelper::initialize()
{
    return m_player;
}

int AdplugHelper::bitrate() const
{
    return (QFileInfo(m_filePath).size() * 8.0) / length() + 1.0f;
}

QStringList AdplugHelper::instruments() const
{
    QStringList insts;
    for(unsigned int i = 0; i < instrument_count(); i++)
    {
        insts << QString::fromStdString(m_player->getinstrument(i));
    }
    return insts;
}
