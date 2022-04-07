#include "musicplsconfigmanager.h"

MusicPLSConfigManager::MusicPLSConfigManager()
    : MusicPlaylistReader()
    , MusicPlaylistInterface()
{

}

bool MusicPLSConfigManager::readBuffer(MusicSongItemList &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file.fileName()).baseName();

    QStringList data(QString(m_file.readAll()).split("\n"));
    if(data.isEmpty())
    {
        return false;
    }

    if(!data.takeFirst().toLower().contains("[playlist]"))
    {
        return false;
    }

    QRegExp fileRegExp("^File(\\d+)=(.+)");
    QRegExp lengthRegExp("^Length(\\d+)=(-{0,1}\\d+)");

    int number = 0;
    bool error = false;

    for(const QString &line : qAsConst(data))
    {
        if(fileRegExp.indexIn(line) > -1)
        {
            if((number = fileRegExp.cap(1).toInt()) > 0)
            {
                item.m_songs << MusicSong(fileRegExp.cap(2));
            }
            else
            {
                error = true;
            }
        }
        else if(lengthRegExp.indexIn(line) > -1)
        {
            if((number = lengthRegExp.cap(1).toInt()) > 0)
            {
                item.m_songs.back().setMusicPlayTime(MusicTime::msecTime2LabelJustified(lengthRegExp.cap(2).toInt() * 1000));
            }
            else
            {
                error = true;
            }
        }

        if(error)
        {
            TTK_LOGGER_ERROR("read pls format playlist error");
            break;
        }
    }
    m_file.close();

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
    return true;
}

bool MusicPLSConfigManager::writeBuffer(const MusicSongItemList &items, const QString &path)
{
    if(items.isEmpty())
    {
        return false;
    }

    const MusicSongItem &item = items.front();
    QStringList data;
    data << QString("[playlist]");

    int count = 1;
    for(const MusicSong &song : qAsConst(item.m_songs))
    {
        data << QString("File%1=%2").arg(count).arg(song.musicPath());
        data << QString("Title%1=%2").arg(count).arg(song.musicName());
        data << QString("Length%1=%2").arg(count).arg(MusicTime::MusicTime::labelJustified2MsecTime(song.musicPlayTime()) / 1000);
        ++count;
    }
    data << "NumberOfEntries=" + QString::number(item.m_songs.count());
    data << "Version=2";

    m_file.setFileName(path);
    if(m_file.open(QFile::WriteOnly))
    {
        m_file.write(data.join("\n").toUtf8());
        m_file.close();
    }
    return true;
}
