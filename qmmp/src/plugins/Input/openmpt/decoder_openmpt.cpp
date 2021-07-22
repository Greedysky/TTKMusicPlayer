#include "decoder_openmpt.h"

#include <QSettings>

DecoderOpenMPT::DecoderOpenMPT(QIODevice *input)
    : Decoder(input)
{

}

DecoderOpenMPT::~DecoderOpenMPT()
{
    delete m_helper;
}

bool DecoderOpenMPT::initialize()
{
    if(!input())
        return false;

    if(!input()->isOpen() && !input()->open(QIODevice::ReadOnly))
        return false;

    m_helper = new OpenMPTHelper(input());
    if(!m_helper->initialize())
    {
        qWarning("DecoderOpenMPT: initialize failed");
        return false;
    }

    configure(m_helper->rate(), m_helper->channels(), Qmmp::PCM_FLOAT);
    qDebug("DecoderOpenMPT: initialize succes");
    return true;
}

qint64 DecoderOpenMPT::totalTime() const
{
    return m_helper->length();
}

int DecoderOpenMPT::bitrate() const
{
    return m_helper->channelCount();
}

qint64 DecoderOpenMPT::read(unsigned char *data, qint64 maxSize)
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("OpenMPT");
    m_helper->setInterpolator(settings.value("interpolator", 8).toInt());
    m_helper->setStereoSeparation(settings.value("stereo_separation", 70).toInt());
    settings.endGroup();

    return m_helper->read(data, maxSize);
}

void DecoderOpenMPT::seek(qint64 pos)
{
    m_helper->seek(pos);
}
