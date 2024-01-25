#include "musictxtconfigmanager.h"

MusicTXTConfigManager::MusicTXTConfigManager()
    : MusicPlaylistRenderer()
    , MusicPlaylistInterface()
{

}

bool MusicTXTConfigManager::readBuffer(MusicSongItemList &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file.fileName()).baseName();

    const QStringList data(QString(m_file.readAll()).split("\n"));
    if(data.isEmpty())
    {
        return false;
    }

    for(const QString &line : qAsConst(data))
    {
        int index = line.indexOf(":");
        if(index != -1)
        {
            const QString &part = line.mid(index + 1).trimmed();
            index = part.lastIndexOf(TTK_DEFAULT_STR);
            if(index != -1)
            {
                item.m_songs << MusicSong(part.left(index).trimmed(), part.mid(index + 1).trimmed());
            }
        }
    }

    m_file.close();

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
    return true;
}

bool MusicTXTConfigManager::writeBuffer(const MusicSongItemList &items)
{
    if(items.isEmpty())
    {
        return false;
    }

    QStringList data;

    int count = 0;
    for(int i = 0; i < items.count(); ++i)
    {
        const MusicSongItem &item = items[i];
        for(const MusicSong &song : qAsConst(item.m_songs))
        {
            data << QString("%1:%2 - %3").arg(++count).arg(song.path(), song.duration());
        }
    }

    m_file.write(data.join("\n").toUtf8());
    m_file.close();
    return true;
}
