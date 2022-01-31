#include "adplughelper.h"

#include <qmmp/qmmp.h>

AdplugHelper::AdplugHelper(const QString &path)
    : m_path(path)
{
    m_opl = new CEmuopl(sampleRate(), true, false);
    m_player = CAdPlug::factory(QmmpPrintable(path), m_opl);
}

AdplugHelper::~AdplugHelper()
{
    deinit();
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
        m_remaining = sampleRate() / m_player->getrefresh();
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

void AdplugHelper::deinit()
{
    delete m_opl;
    delete m_player;
}

QStringList AdplugHelper::instruments() const
{
    QStringList insts;
    for(unsigned int i = 0; i < instrumentCount(); i++)
    {
        insts << QString::fromStdString(m_player->getinstrument(i));
    }
    return insts;
}
