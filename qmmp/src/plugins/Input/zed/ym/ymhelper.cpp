#include "ymhelper.h"

YMHelper::YMHelper(const QString &path)
    : m_path(path)
{

}

YMHelper::~YMHelper()
{
    deinit();
}

void YMHelper::deinit()
{
    if(m_music)
    {
        delete m_music;
    }
}

bool YMHelper::initialize()
{
    m_music = new CYmMusic;
    if(!m_music)
    {
        qWarning("YMHelper: failed to create CYmMusic");
        return false;
    }

    ymMusicInfo_t info;
    if(m_music->load(QmmpPrintable(m_path)))
    {
        m_music->getMusicInfo(&info);
        m_music->setLoopMode(YMFALSE);
        m_length = info.musicTimeInMs;
    }
    else
    {
        if(m_music)
        {
            delete m_music;
        }

        m_music = nullptr;
        qWarning("YMHelper: failed to open: %s", QmmpPrintable(m_path));
        return false;
    }
    return true;
}

qint64 YMHelper::read(unsigned char *data, qint64 maxSize)
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

QMap<Qmmp::MetaData, QString> YMHelper::readMetaData() const
{
    QMap<Qmmp::MetaData, QString> metaData;
    if(!m_music)
    {
        return metaData;
    }

    ymMusicInfo_t musicInfo;
    m_music->getMusicInfo(&musicInfo);

    char* title = strdup(musicInfo.pSongName);
    char* composer = strdup(musicInfo.pSongAuthor);
    char* comment = strdup(musicInfo.pSongComment);

    metaData.insert(Qmmp::TITLE, QString::fromUtf8(title).trimmed());
    metaData.insert(Qmmp::COMPOSER, QString::fromUtf8(composer).trimmed());
    metaData.insert(Qmmp::COMMENT, QString::fromUtf8(comment).trimmed());
    return metaData;
}
