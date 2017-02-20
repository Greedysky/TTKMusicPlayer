#include "ttkmusicsongssummarizied.h"
#include "musiccryptographichash.h"

TTKMusicSongsSummarizied::TTKMusicSongsSummarizied(QObject *parent)
    : QObject(parent)
{
    m_currentPlayIndex = MUSIC_NORMAL_LIST;
    m_currentToolIndex = MUSIC_NORMAL_LIST;
}

void TTKMusicSongsSummarizied::addMusicLists(const MusicSongItems &names)
{
    m_songItems = names;
}

void TTKMusicSongsSummarizied::appendMusicLists(const MusicSongItems &names)
{
    m_songItems << names;
}

MusicSongItem TTKMusicSongsSummarizied::getMusicList(int index)
{
    if(index < MUSIC_NORMAL_LIST || index >= m_songItems.count())
    {
        return MusicSongItem();
    }
    return m_songItems[index];
}

void TTKMusicSongsSummarizied::importOtherMusicSongs(int index, const QStringList &filelist)
{
    if(index < MUSIC_NORMAL_LIST || index >= m_songItems.count())
    {
        return;
    }

    MusicSongItem *item = &m_songItems[index];
    foreach(const QString &path, filelist)
    {
        MusicSong music(path);
        if(item->m_songs.contains(music))
        {
            continue;
        }
        item->m_songs << music;
    }
}

void TTKMusicSongsSummarizied::importNetworkMusicSongs(const QString &key, const QString &path)
{
    QString musicSong = MusicCryptographicHash().decrypt(key, DOWNLOAD_KEY);
    MusicSongItem *item = &m_songItems[MUSIC_RECENT_LIST];
    MusicSong music(path, 0, QString(), musicSong);
    if(!item->m_songs.contains(music))
    {
        item->m_songs << music;
    }
}

void TTKMusicSongsSummarizied::setRecentMusicSongs(int index)
{
    if(m_currentPlayIndex < MUSIC_NORMAL_LIST || m_currentPlayIndex >= m_songItems.count())
    {
        return;
    }
    MusicSongs songs(m_songItems[m_currentPlayIndex].m_songs);
    if(index < MUSIC_NORMAL_LIST || index >= songs.count())
    {
        return;
    }

    MusicSongItem *item = &m_songItems[MUSIC_RECENT_LIST];
    MusicSong music( songs[index] );
    if(!item->m_songs.contains(music))
    {
        item->m_songs << music;
    }

    MusicSongs *musics = &item->m_songs;
    for(int i=0; i<musics->count(); ++i)
    {
        MusicSong *m = &(*musics)[i];
        if(music == *m)
        {
            m->setMusicPlayCount(m->getMusicPlayCount() + 1);
            break;
        }
    }
}

void TTKMusicSongsSummarizied::removeMusicSongs(int tool, int index)
{
    if(tool < MUSIC_NORMAL_LIST || tool >= m_songItems.count())
    {
        return;
    }
    MusicSongs songs(m_songItems[tool].m_songs);
    if(index < MUSIC_NORMAL_LIST || index >= songs.count())
    {
        return;
    }

    MusicSongItem *item = &m_songItems[tool];
    item->m_songs.takeAt(index);
}

void TTKMusicSongsSummarizied::removeMusicSongs(int tool, const QString &path)
{
    if(tool < MUSIC_NORMAL_LIST || tool >= m_songItems.count())
    {
        return;
    }

    MusicSongItem *item = &m_songItems[tool];
    item->m_songs.removeOne(MusicSong(path));
}

QStringList TTKMusicSongsSummarizied::getMusicSongsFileName(int index) const
{
    QStringList list;
    if(index < MUSIC_NORMAL_LIST || index >= m_songItems.count())
    {
        return list;
    }

    foreach(const MusicSong &song, m_songItems[index].m_songs)
    {
        list << song.getMusicName();
    }
    return list;
}

QStringList TTKMusicSongsSummarizied::getMusicSongsFilePath(int index) const
{
    QStringList list;
    if(index < MUSIC_NORMAL_LIST || index >= m_songItems.count())
    {
        return list;
    }

    foreach(const MusicSong &song, m_songItems[index].m_songs)
    {
        list << song.getMusicPath();
    }
    return list;
}
