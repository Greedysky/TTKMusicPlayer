#include "decoder_uade.h"
#include "uadehelper.h"

DecoderUADE::DecoderUADE(const QString &path)
    : Decoder(),
      m_path(path)
{

}

DecoderUADE::~DecoderUADE()
{

}

bool DecoderUADE::initialize()
{
    if(!UADEHelper::instance()->initialize(m_path, true))
    {
        qWarning("DecoderUADE: initialize failed");
        return false;
    }

    const int rate = UADEHelper::instance()->sampleRate();
    const int channels = UADEHelper::instance()->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderUADE: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderUADE: initialize success");
    return true;
}

qint64 DecoderUADE::totalTime() const
{
    return UADEHelper::instance()->totalTime();
}

int DecoderUADE::bitrate() const
{
    return UADEHelper::instance()->bitrate();
}

qint64 DecoderUADE::read(unsigned char *data, qint64 maxSize)
{
    return UADEHelper::instance()->read(data, maxSize);
}

void DecoderUADE::seek(qint64 time)
{
    UADEHelper::instance()->seek(time);
}
