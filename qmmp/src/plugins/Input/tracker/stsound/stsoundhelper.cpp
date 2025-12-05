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
    delete m_input;
}

bool StSoundHelper::initialize()
{
    m_input = new CYmMusic;
    if(!m_input)
    {
        qWarning("StSoundHelper: failed to create CYmMusic");
        return false;
    }

    if(m_input->load(QmmpPrintable(m_path)))
    {
        ymMusicInfo_t info;
        m_input->getMusicInfo(&info);
        m_input->setLoopMode(YMFALSE);

        m_length = info.musicTimeInMs;
        m_title = QString::fromUtf8(info.pSongName);
        m_author = QString::fromUtf8(info.pSongAuthor);
        m_comment =  QString::fromUtf8(info.pSongComment);
    }
    else
    {
        delete m_input;
        m_input = nullptr;
        qWarning("StSoundHelper: failed to open: %s", QmmpPrintable(m_path));
        return false;
    }
    return true;
}

qint64 StSoundHelper::read(unsigned char *data, qint64 maxSize)
{
    ymsample *psample = (ymsample *)data;
    const qint64 stereoSize = maxSize / (channels() * sizeof(ymsample));

    if(!m_input->update(psample, stereoSize))
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
