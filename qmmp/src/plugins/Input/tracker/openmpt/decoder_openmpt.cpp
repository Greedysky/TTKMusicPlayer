#include "decoder_openmpt.h"

#include <QSettings>

DecoderOpenMPT *DecoderOpenMPT::m_instance = nullptr;

DecoderOpenMPT::DecoderOpenMPT(QIODevice *input)
    : Decoder(input)
{
    m_instance = this;
}

DecoderOpenMPT::~DecoderOpenMPT()
{
    m_instance = nullptr;
    delete m_helper;
}

DecoderOpenMPT *DecoderOpenMPT::instance()
{
    return m_instance;
}

void DecoderOpenMPT::readSettings()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("OpenMPT");
    m_helper->setInterpolator(settings.value("interpolator", INTERP_WINDOWED).toInt());
    m_helper->setStereoSeparation(settings.value("stereo_separation", 100).toInt());
    settings.endGroup();
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

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderOpenMPT: rate or channel invalid");
        return false;
    }

    readSettings();

    configure(rate, channels, Qmmp::PCM_FLOAT);
    qDebug("DecoderOpenMPT: initialize success");
    return true;
}

qint64 DecoderOpenMPT::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderOpenMPT::bitrate() const
{
    return m_helper->channelCount();
}

qint64 DecoderOpenMPT::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderOpenMPT::seek(qint64 time)
{
    m_helper->seek(time);
}
