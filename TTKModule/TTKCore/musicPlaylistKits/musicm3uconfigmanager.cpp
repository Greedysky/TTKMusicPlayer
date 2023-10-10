#include "musicm3uconfigmanager.h"

MusicM3UConfigManager::MusicM3UConfigManager()
    : MusicPlaylistRenderer()
    , MusicPlaylistInterface()
{

}

bool MusicM3UConfigManager::readBuffer(MusicSongItemList &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file.fileName()).baseName();

    const QStringList data(QString(m_file.readAll()).split("\n"));
    if(data.isEmpty())
    {
        return false;
    }

    int length = 0;
    bool valid = false;
    const QRegExp regx("#EXTINF:(-{0,1}\\d+),(.*)");

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
            item.m_songs << MusicSong(str, TTKTime::formatDuration(length * MT_S2MS));
        }
    }

    m_file.close();

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
    return true;
}

bool MusicM3UConfigManager::writeBuffer(const MusicSongItemList &items)
{
    if(items.isEmpty())
    {
        return false;
    }

    QStringList data;
    data << QString("#EXTM3U");

    for(int i = 0; i < items.count(); ++i)
    {
        const MusicSongItem &item = items[i];
        for(const MusicSong &song : qAsConst(item.m_songs))
        {
            data.append(QString("#EXTINF:%1,%2 - %3").arg(TTKTime::formatDuration(song.playTime()) / MT_S2MS)
                                                     .arg(song.artistFront(), song.artistBack()));
            data.append(song.path());
        }
    }

    m_file.write(data.join("\n").toUtf8());
    m_file.close();
    return true;
}
