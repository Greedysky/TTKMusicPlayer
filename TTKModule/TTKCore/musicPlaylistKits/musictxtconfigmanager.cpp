#include "musictxtconfigmanager.h"

MusicTXTConfigManager::MusicTXTConfigManager()
    : MusicPlaylistInterface()
{

}

bool MusicTXTConfigManager::readConfig(const QString &name)
{
    m_file.setFileName(name);
    return m_file.open(QFile::ReadOnly);
}

bool MusicTXTConfigManager::readPlaylistData(MusicSongItems &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file.fileName()).baseName();

    QStringList data(QString(m_file.readAll()).split("\n"));
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
            index = part.lastIndexOf("-");
            if(index != -1)
            {
                MusicSong song(part.left(index).trimmed());
                song.setMusicPlayTime(part.mid(index + 1).trimmed());
                item.m_songs << song;
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

bool MusicTXTConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    if(items.isEmpty())
    {
        return false;
    }

    const MusicSongItem &item = items.first();
    QStringList data;
    for(int i=0; i<item.m_songs.count(); ++i)
    {
        data << QString("%1:%2 - %3").arg(i).arg(item.m_songs[i].getMusicPath()).arg(item.m_songs[i].getMusicPlayTime());
    }

    m_file.setFileName(path);
    if(m_file.open(QFile::WriteOnly))
    {
        m_file.write(data.join("\n").toUtf8());
        m_file.close();
    }
    return true;
}
