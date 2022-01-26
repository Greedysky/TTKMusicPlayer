#include "decoder_ym.h"

#include <QFileInfo>

DecoderYm::DecoderYm(const QString &path)
    : Decoder(),
      m_path(path)
{

}

DecoderYm::~DecoderYm()
{
    deinit();
}

bool DecoderYm::initialize()
{
    m_totalTime = 0;
    m_freq = 0;
    m_bitrate = 0;

    m_music = new CYmMusic;
    if(!m_music)
    {
        qWarning("DecoderYm: failed to create CYmMusic");
        return false;
    }

    ymMusicInfo_t info;
    if(m_music->load(QmmpPrintable(m_path)))
    {
        m_music->getMusicInfo(&info);
        m_music->setLoopMode(YMFALSE);

        m_totalTime = info.musicTimeInMs;
        m_freq = 44100;
        m_bitrate = (QFileInfo(m_path).size() * 8.0) / m_totalTime + 1.0f;
    }
    else
    {
        if(m_music)
        {
            delete m_music;
        }
        m_music = nullptr;
        qWarning("DecoderYm: failed to open: %s", QmmpPrintable(m_path));
        return false;
    }

    configure(m_freq, 2, Qmmp::PCM_S16LE);
    qDebug("DecoderYm: detected format: \"%s\"", info.pSongType);
    qDebug("DecoderYm: initialize success");
    return true;
}

void DecoderYm::deinit()
{
    m_totalTime = 0;
    m_bitrate = 0;
    m_freq = 0;
    if(m_music)
    {
        delete m_music;
    }
    m_music = nullptr;
}

qint64 DecoderYm::totalTime() const
{
    return m_totalTime;
}

int DecoderYm::bitrate() const
{
    return m_bitrate ? m_bitrate : 0;
}

qint64 DecoderYm::read(unsigned char *data, qint64 maxSize)
{
    qint64 stereoSize, i;
    ymsample *psample = (ymsample *)data;
    stereoSize = maxSize / (2 * sizeof(ymsample));
    
    if(m_music->update(psample, stereoSize))
    {
        // recopy mono YM sound to 2 channels
        for(i=stereoSize-1; i>=0; i--)
        {
            psample[(i * 2)    ] = psample[i];
            psample[(i * 2) + 1] = psample[i];
        }
        return maxSize;
    }
    return 0;
}

void DecoderYm::seek(qint64 time)
{
    m_music->setMusicTime((ymu32)time);
}
