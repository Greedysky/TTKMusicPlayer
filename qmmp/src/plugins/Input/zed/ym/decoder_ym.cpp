#include "decoder_ym.h"
#include "ymhelper.h"

DecoderYm::DecoderYm(const QString &path)
    : Decoder()
{
    m_helper = new YMHelper(path);
}

DecoderYm::~DecoderYm()
{
    delete m_helper;
}

bool DecoderYm::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderYm: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderYm: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderYm: initialize success");
    return true;
}

qint64 DecoderYm::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderYm::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderYm::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderYm::seek(qint64 time)
{
    m_helper->seek(time);
}
