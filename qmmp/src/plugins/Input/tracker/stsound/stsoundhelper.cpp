#include "stsoundhelper.h"

StSoundHelper::StSoundHelper(const QString &path)
    : m_path(path)
{

}

StSoundHelper::~StSoundHelper()
{
    deinit();
}

void StSoundHelper::deinit()
{
    if(m_music)
    {
        delete m_music;
    }
}

bool StSoundHelper::initialize()
{
    m_music = new CYmMusic;
    if(!m_music)
    {
        qWarning("StSoundHelper: failed to create CYmMusic");
        return false;
    }

    if(m_music->load(QmmpPrintable(m_path)))
    {
        ymMusicInfo_t info;
        m_music->getMusicInfo(&info);
        m_music->setLoopMode(YMFALSE);

        m_length = info.musicTimeInMs;
        m_title = QString::fromUtf8(info.pSongName);
        m_author = QString::fromUtf8(info.pSongAuthor);
        m_comment =  QString::fromUtf8(info.pSongComment);
    }
    else
    {
        if(m_music)
        {
            delete m_music;
        }

        m_music = nullptr;
        qWarning("StSoundHelper: failed to open: %s", QmmpPrintable(m_path));
        return false;
    }
    return true;
}

qint64 StSoundHelper::read(unsigned char *data, qint64 maxSize)
{
    ymsample *psample = (ymsample *)data;
    const qint64 stereoSize = maxSize / (2 * sizeof(ymsample));

    if(!m_music->update(psample, stereoSize))
    {
        return 0;
    }
    // recopy mono YM sound to 2 channels
    for(qint64 i = stereoSize - 1; i >= 0; --i)
    {
        psample[(i * 2)    ] = psample[i];
        psample[(i * 2) + 1] = psample[i];
    }
    return maxSize;
}
