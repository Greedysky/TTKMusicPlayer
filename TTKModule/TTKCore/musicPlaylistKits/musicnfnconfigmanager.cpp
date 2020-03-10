#include "musicnfnconfigmanager.h"

MusicNFNConfigManager::MusicNFNConfigManager()
    : MusicPlaylistInterface()
{

}

bool MusicNFNConfigManager::readPlaylistData(MusicSongItems &items)
{
    Q_UNUSED(items);
    return false;
}

bool MusicNFNConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    if(items.isEmpty())
    {
        return false;
    }

    QDir().mkpath(path);
    const MusicSongItem &item = items.first();
    for(int i=0; i<item.m_songs.count(); ++i)
    {
        const MusicSong &song = item.m_songs[i];
        const QString &dest = QString("%1/%2.%3").arg(path).arg(song.getMusicName()).arg(song.getMusicType());
        QFile::copy(song.getMusicPath(), dest);
    }
    return true;
}
