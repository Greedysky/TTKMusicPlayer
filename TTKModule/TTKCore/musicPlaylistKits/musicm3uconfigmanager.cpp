#include "musicm3uconfigmanager.h"
#include "musictime.h"

MusicM3UConfigManager::MusicM3UConfigManager()
    : MusicPlaylistInterface()
{

}

bool MusicM3UConfigManager::readConfig(const QString &name)
{
    m_file.setFileName(name);
    return m_file.open(QFile::ReadOnly);
}

void MusicM3UConfigManager::readPlaylistData(MusicSongItems &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file.fileName()).baseName();

    const QStringList data(QString(m_file.readAll()).split("\n"));

    QRegExp regx("#EXTINF:(-{0,1}\\d+),(.*) - (.*)");
    int length = 0;
    bool hasExtInf = false;

    foreach(QString str, data)
    {
        str = str.trimmed();
        if(str.startsWith("#EXTM3U") || str.isEmpty())
        {
            continue;
        }

        if(regx.indexIn(str) > -1)
        {
            length = regx.cap(1).toInt();
            hasExtInf = true;
        }

        if(str.startsWith("#"))
        {
            continue;
        }

        item.m_songs << MusicSong(str, 0,
                                  hasExtInf ? MusicTime::msecTime2LabelJustified(length * MT_S2MS) : QString(),
                                  QString());
    }

    m_file.close();

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
}

void MusicM3UConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    if(items.isEmpty())
    {
        return;
    }

    const MusicSongItem &item = items.first();
    QStringList data;
    data << QString("#EXTM3U");
    foreach(const MusicSong &song, item.m_songs)
    {
        data.append(QString("#EXTINF:%1,%2 - %3").arg(MusicTime::labelJustified2MsecTime(song.getMusicPlayTime())/1000)
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
}
