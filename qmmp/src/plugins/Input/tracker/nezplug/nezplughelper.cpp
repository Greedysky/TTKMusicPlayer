#include "nezplughelper.h"

#include <QSettings>

NEZplugHelper::NEZplugHelper(const QString &path)
    : m_path(path)
{

}

NEZplugHelper::~NEZplugHelper()
{
    deinit();
}

void NEZplugHelper::deinit()
{
    if(m_input)
    {
        NEZDelete(m_input);
    }
}

bool NEZplugHelper::initialize()
{
    const QString &path = cleanPath();

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("NEZplugHelper: open file failed");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    m_input = NEZNew();
    if(!m_input)
    {
        qWarning("NEZplugHelper: NEZNew failed");
        return false;
    }

    if(NEZLoad(m_input, (uint8_t*)buffer.constData(), buffer.length()))
    {
        qWarning("NEZplugHelper: NEZLoad failed");
        return false;
    }

    NEZSetFrequency(m_input, sampleRate());
    NEZSetChannel(m_input, channels());

    const int track = m_path.section("#", -1).toInt();
    NEZSetSongNo(m_input, track < 1 ? 1 : track);
    NEZReset(m_input);
    return true;
}

qint64 NEZplugHelper::totalTime() const
{
    const int track = m_path.section("#", -1).toInt();
    return NEZGetTrackLength(m_input, track < 1 ? 1 : track);
}

qint64 NEZplugHelper::read(unsigned char *data, qint64 maxSize)
{
    NEZRender(m_input, data, maxSize / 4);
    return maxSize;
}

QList<TrackInfo*> NEZplugHelper::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> playlist;
    if(!m_input)
    {
        return playlist;
    }

    const int songs = NEZGetSongMaxAbsolute(m_input) - NEZGetSongStart(m_input) + 1;
    for(int i = 1; i <= songs; ++i)
    {
        NEZSetSongNo(m_input, i);
        NEZReset(m_input);

        TrackInfo *info = new TrackInfo();
        if(parts & TrackInfo::MetaData)
        {
            info->setValue(Qmmp::TITLE, NEZGetTrackTitle(m_input, i));
            info->setValue(Qmmp::ARTIST, NEZGetGameArtist(m_input));
            info->setValue(Qmmp::ALBUM, NEZGetGameTitle(m_input));
            info->setValue(Qmmp::TRACK, i);
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, bitrate());
            info->setValue(Qmmp::SAMPLERATE, sampleRate());
            info->setValue(Qmmp::CHANNELS, channels());
            info->setValue(Qmmp::BITS_PER_SAMPLE, depth());
            info->setValue(Qmmp::FORMAT_NAME, NEZGetGameDetail(m_input));
        }

        info->setPath("nezplug://" + cleanPath() + QString("#%1").arg(i));
        info->setDuration(NEZGetTrackLength(m_input, i));
        playlist << info;
    }
    return playlist;
}

QString NEZplugHelper::cleanPath() const
{
    return m_path.contains("://") ? TrackInfo::pathFromUrl(m_path) : m_path;
}
