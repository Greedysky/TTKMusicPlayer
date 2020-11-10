#include "musicm3uconfigmanager.h"

MusicM3UConfigManager::MusicM3UConfigManager()
    : MusicPlaylistInterface()
{

}

bool MusicM3UConfigManager::readConfig(const QString &name)
{
    m_file.setFileName(name);
    return m_file.open(QFile::ReadOnly);
}

bool MusicM3UConfigManager::readPlaylistData(MusicSongItems &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file.fileName()).baseName();

    const QStringList data(QString(m_file.readAll()).split("\n"));

    QRegExp regx("#EXTINF:(-{0,1}\\d+),(.*)");
    int length = 0;
    bool valid = false;

    for(QString str : qAsConst(data))
    {
        str = str.trimmed();
        if(str.startsWith("#EXTM3U") || str.isEmpty())
        {
            continue;
        }

        if(regx.indexIn(str) > -1)
        {
            length = regx.cap(1).toInt();
            valid = true;
        }

        if(str.startsWith("#"))
        {
            continue;
        }

        if(valid)
        {
            item.m_songs << MusicSong(str, 0, MusicTime::msecTime2LabelJustified(length * MT_S2MS), QString());
        }
    }

    m_file.close();

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
    return true;
}

bool MusicM3UConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    if(items.isEmpty())
    {
        return false;
    }

    const MusicSongItem &item = items.first();
    QStringList data;
    data << QString("#EXTM3U");
    for(const MusicSong &song : qAsConst(item.m_songs))
    {
        data.append(QString("#EXTINF:%1,%2 - %3").arg(MusicTime::labelJustified2MsecTime(song.getMusicPlayTime()) / MT_S2MS)
                                                 .arg(song.getMusicArtistFront())
                                                 .arg(song.getMusicArtistBack()));
        data.append(song.getMusicPath());
    }

    m_file.setFileName(path);
    if(m_file.open(QFile::WriteOnly))
    {
        m_file.write(data.join("\n").toUtf8());
        m_file.close();
    }
    return true;
}
