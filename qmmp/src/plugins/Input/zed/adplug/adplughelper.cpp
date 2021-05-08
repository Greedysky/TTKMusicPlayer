#include "adplughelper.h"

#include <QFileInfo>

AdplugHelper::AdplugHelper(const std::string &filename)
    : m_filePath(filename),
      m_opl(new CEmuopl(rate(), true, false)),
      m_player(CAdPlug::factory(filename.c_str(), m_opl.get()))
{

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
    return m_player.get();
}

int AdplugHelper::bitrate()
{
    return (QFileInfo(m_filePath.c_str()).size() * 8.0) / length() + 1.0f;
}

std::vector<std::string> AdplugHelper::instruments()
{
    std::vector<std::string> insts;
    for(unsigned int i = 0; i < instrument_count(); i++)
    {
        insts.push_back(m_player->getinstrument(i));
    }
    return insts;
}
