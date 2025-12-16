#include "adplughelper.h"

#include <qmmp/qmmp.h>
#include <QSettings>

#include <adplug/emuopl.h>
#include <adplug/kemuopl.h>
#include <adplug/nemuopl.h>
#include <adplug/wemuopl.h>
#include <adplug/surroundopl.h>

AdPlugHelper::AdPlugHelper(const QString &path)
    : m_path(path)
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("AdPlug");
    m_sampleRate = settings.value("sample_rate", 44100).toInt();
    const int type = settings.value("emulator", 0).toInt();
    const bool surround = settings.value("use_surround", false).toBool();
    settings.endGroup();

    switch(type)
    {
    case 0:
        if(surround)
        {
            COPLprops a;
            a.opl = new CEmuopl(sampleRate(), true, false);
            a.use16bit = true;
            a.stereo = true;

            COPLprops b;
            b.opl = new CEmuopl(sampleRate(), true, false);
            b.use16bit = true;
            b.stereo = true;
            m_opl = new CSurroundopl(&a, &b, true);
        }
        else
        {
            m_opl = new CEmuopl(sampleRate(), true, false);
        }
        break;
    case 1:
        if(surround)
        {
            COPLprops a;
            a.opl = new CNemuopl(sampleRate());
            a.use16bit = true;
            a.stereo = true;

            COPLprops b;
            b.opl = new CNemuopl(sampleRate());
            b.use16bit = true;
            b.stereo = true;
            m_opl = new CSurroundopl(&a, &b, true);
        }
        else
        {
            m_opl = new CNemuopl(sampleRate());
        }
        break;
    case 2:
        if(surround)
        {
            COPLprops a;
            a.opl = new CWemuopl(sampleRate(), true, false);
            a.use16bit = true;
            a.stereo = true;

            COPLprops b;
            b.opl = new CWemuopl(sampleRate(), true, false);
            b.use16bit = true;
            b.stereo = true;
            m_opl = new CSurroundopl(&a, &b, true);
        }
        else
        {
            m_opl = new CWemuopl(sampleRate(), true, false);
        }
        break;
    case 3:
        if(surround)
        {
            COPLprops a;
            a.opl = new CKemuopl(sampleRate(), true, false);
            a.use16bit = true;
            a.stereo = true;

            COPLprops b;
            b.opl = new CKemuopl(sampleRate(), true, false);
            b.use16bit = true;
            b.stereo = true;
            m_opl = new CSurroundopl(&a, &b, true);
        }
        else
        {
            m_opl = new CKemuopl(sampleRate(), true, false);
        }
        break;
    default:
        break;
    }

    m_player = CAdPlug::factory(QmmpPrintable(path), m_opl);
}

AdPlugHelper::~AdPlugHelper()
{
    deinit();
}

AdPlugHelper::Frame AdPlugHelper::read()
{
    size_t to_write;
    const size_t bufsiz = sizeof(m_buf) / sizeof(*m_buf);

    if(m_remaining == 0)
    {
        if(!m_player->update())
        {
            return Frame(0, nullptr);
        }
        m_remaining = sampleRate() / m_player->getrefresh();
    }

    to_write = m_remaining > bufsiz ? bufsiz : m_remaining;
    m_opl->update(m_buf, to_write);
    m_remaining -= to_write;
    return Frame(to_write * 2, reinterpret_cast<unsigned char*>(m_buf));
}

void AdPlugHelper::deinit()
{
    delete m_opl;
    delete m_player;
}
