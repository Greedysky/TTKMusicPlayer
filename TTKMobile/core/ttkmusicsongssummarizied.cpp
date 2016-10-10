#include "ttkmusicsongssummarizied.h"
#include "musicsongtag.h"

TTKMusicSongsSummarizied::TTKMusicSongsSummarizied(QObject *parent)
    : QObject(parent)
{
    m_currentPlayIndex = 0;
    m_currentToolIndex = 0;
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
    if(index < 0 || index >= m_songItems.count())
    {
        return MusicSongItem();
    }
    return m_songItems[index];
}

void TTKMusicSongsSummarizied::importOtherMusicSongs(const QStringList &filelist)
{
    if(m_currentPlayIndex < 0 || m_currentPlayIndex >= m_songItems.count())
    {
        return;
    }

    QStringList l(filelist);
    MusicSongTag tag;
    MusicSongItem *item = &m_songItems[m_currentPlayIndex];
    foreach(const QString &path, l)
    {
        if(item->m_songs.contains(MusicSong(path)))
        {
            l.removeAll(path);
            continue;
        }
        QString time = tag.readFile(path) ? tag.getLengthString() : "-";
        item->m_songs << MusicSong(path, 0, time, QString());
    }
}

QStringList TTKMusicSongsSummarizied::getMusicSongsFileName(int index) const
{
    QStringList list;
    if(index < 0 || index >= m_songItems.count())
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
    if(index < 0 || index >= m_songItems.count())
    {
        return list;
    }

    foreach(const MusicSong &song, m_songItems[index].m_songs)
    {
        list << song.getMusicPath();
    }
    return list;
}
