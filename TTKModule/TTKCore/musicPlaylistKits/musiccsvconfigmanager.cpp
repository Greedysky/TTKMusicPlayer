#include "musiccsvconfigmanager.h"

MusicCSVConfigManager::MusicCSVConfigManager()
    : MusicPlaylistRenderer()
    , MusicPlaylistInterface()
{

}

bool MusicCSVConfigManager::readBuffer(MusicSongItemList &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file.fileName()).baseName();

    const QStringList data(QString(m_file.readAll()).split(TTK_LINEFEED));
    if(data.isEmpty())
    {
        return false;
    }

    for(const QString &line : qAsConst(data))
    {
        const QStringList &songInfo = line.split(",");
        if(songInfo.count() > 2)
        {
            item.m_songs << MusicSong(songInfo[2], songInfo[1], songInfo[0]);
        }
    }

    m_file.close();

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
    return true;
}

bool MusicCSVConfigManager::writeBuffer(const MusicSongItemList &items)
{
    if(items.isEmpty())
    {
        return false;
    }

    QStringList data;
    for(int i = 0; i < items.count(); ++i)
    {
        const MusicSongItem &item = items[i];
        for(const MusicSong &song : qAsConst(item.m_songs))
        {
            data << song.name() + "," + song.duration() + "," + song.path();
        }
    }

    m_file.write(data.join(TTK_LINEFEED).toUtf8());
    m_file.close();
    return true;
}
