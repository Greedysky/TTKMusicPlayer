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

    QRegExp regx1("^File(\\d+)=(.+)");
    QRegExp regx2("^Length(\\d+)=(-{0,1}\\d+)");

    int number = 0;
    bool error = false;

    for(const QString &line : qAsConst(data))
    {
        if(regx1.indexIn(line) > -1)
        {
            if((number = regx1.cap(1).toInt()) > 0)
            {
                item.m_songs << MusicSong(regx1.cap(2));
            }
            else
            {
                error = true;
            }
        }
        else if(regx2.indexIn(line) > -1)
        {
            if((number = regx2.cap(1).toInt()) > 0)
            {
                item.m_songs.back().setPlayTime(TTKTime::msecTime2LabelJustified(regx2.cap(2).toInt() * 1000));
            }
            else
            {
                error = true;
            }
        }

        if(error)
        {
            TTK_ERROR_STREAM("read pls format playlist error");
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
        data << QString("File%1=%2").arg(count).arg(song.path());
        data << QString("Title%1=%2").arg(count).arg(song.name());
        data << QString("Length%1=%2").arg(count).arg(TTKTime::TTKTime::labelJustified2MsecTime(song.playTime()) / 1000);
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
