#include "musicm3uconfigmanager.h"
#include "musicplaylistmanager.h"

MusicM3UConfigManager::MusicM3UConfigManager()
    : MusicPlaylistInterface()
{

}

bool MusicM3UConfigManager::readConfig(const QString &name)
{
    m_file.setFileName(name);
    return m_file.open(QFile::ReadOnly);
}

void MusicM3UConfigManager::readPlaylistConfig(MusicSongItems &musics)
{
    bool state = false;
    MusicSongItem item;
    const QStringList data(QString(m_file.readAll()).split("\n"));
    foreach(QString str, data)
    {
        str = str.trimmed();
        if(str.isEmpty())
        {
            continue;
        }
        else if(str.startsWith("#TTKM3U"))
        {
            state = true;
            continue;
        }
        else if(str.startsWith("#TTKNAME:"))
        {
            item.m_itemName = str.remove("#TTKNAME:");
        }
        else if(str.startsWith("#TTKTIT:"))
        {
            str = str.remove("#TTKTIT:");
            const QStringList &dds = str.split(TTK_STR_SPLITER);
            if(dds.count() == 3)
            {
                item.m_itemIndex = dds[0].toInt();
                item.m_sort.m_index = dds[1].toInt();
                item.m_sort.m_sortType = MStatic_cast(Qt::SortOrder, dds[2].toInt());
            }
        }
        else if(str.startsWith("#TTKINF:"))
        {
            str = str.remove("#TTKINF:");
            const QStringList &dds = str.split(TTK_STR_SPLITER);
            if(dds.count() == 4)
            {
                item.m_songs << MusicSong(dds[3], dds[0].toInt(), dds[2], dds[1]);
            }
        }
    }
    m_file.close();

    if(state)
    {
        musics << item;
    }
    else
    {
        MusicPlaylistManager::messageAlert();
    }
}

void MusicM3UConfigManager::writePlaylistConfig(const MusicSongItems &musics, const QString &path)
{
    if(musics.isEmpty())
    {
        return;
    }

    const MusicSongItem &item = musics.first();
    QStringList data;
    data << QString("#TTKM3U");
    data << QString("#TTKNAME:%1").arg(item.m_itemName);
    data << QString("#TTKTIT:%2%1%3%1%4").arg(TTK_STR_SPLITER).arg(item.m_itemIndex)
                                         .arg(item.m_sort.m_index).arg(item.m_sort.m_sortType);
    foreach(const MusicSong &song, item.m_songs)
    {
        data.append(QString("#TTKINF:%2%1%3%1%4%1%5").arg(TTK_STR_SPLITER).arg(song.getMusicPlayCount())
                                                     .arg(song.getMusicName()).arg(song.getMusicPlayTime())
                                                     .arg(song.getMusicPath()));
        data.append(song.getMusicPath());
    }

    m_file.setFileName(path);
    if(m_file.open(QFile::WriteOnly))
    {
        m_file.write(data.join("\n").toUtf8());
        m_file.close();
    }
}
