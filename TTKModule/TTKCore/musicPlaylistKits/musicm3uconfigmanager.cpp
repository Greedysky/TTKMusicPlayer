#include "musicm3uconfigmanager.h"
#include "ttkregularexpression.h"

MusicM3UConfigManager::MusicM3UConfigManager()
    : MusicPlaylistRenderer()
    , MusicPlaylistInterface()
{

}

bool MusicM3UConfigManager::readBuffer(MusicSongItemList &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file.fileName()).baseName();

    const QStringList data(QString(m_file.readAll()).split(TTK_LINEFEED));
    if(data.isEmpty())
    {
        return false;
    }

    int length = 0;
    bool valid = false;
    TTKRegularExpression regx("#EXTINF:(-{0,1}\\d+),(.*)");

    for(QString str : qAsConst(data))
    {
        str = str.trimmed();
        if(str.startsWith("#EXTM3U") || str.isEmpty())
        {
            continue;
        }

        if(regx.match(str) > -1)
        {
            length = regx.captured(1).toInt();
            valid = true;
        }

        if(str.startsWith("#"))
        {
            continue;
        }

        if(valid)
        {
            item.m_songs << MusicSong(str, TTKTime::formatDuration(length * TTK_DN_S2MS));
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
            data.append(QString("#EXTINF:%1,%2 - %3").arg(TTKTime::formatDuration(song.duration()) / TTK_DN_S2MS).arg(song.artist(), song.title()));
            data.append(song.path());
        }
    }

    m_file.write(data.join(TTK_LINEFEED).toUtf8());
    m_file.close();
    return true;
}
