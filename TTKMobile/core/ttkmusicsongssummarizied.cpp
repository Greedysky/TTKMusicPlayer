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

void TTKMusicSongsSummarizied::importOtherMusicSongs(const QStringList &filelist)
{
    if(m_songItems.isEmpty())
    {
        return;
    }

    QStringList l(filelist);
    MusicSongItem *item = &m_songItems[MUSIC_NORMAL_LIST];
    foreach(const QString &path, l)
    {
        MusicSong music(path);
        if(item->m_songs.contains(music))
        {
            continue;
        }
        item->m_songs << music;
    }
}

void TTKMusicSongsSummarizied::importRecentMusicSongs(int index)
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
    MusicSong music( songs[index].getMusicPath() );
    if(!item->m_songs.contains(music))
    {
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
