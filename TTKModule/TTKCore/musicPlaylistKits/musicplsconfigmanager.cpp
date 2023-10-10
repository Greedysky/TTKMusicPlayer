#include "musicplsconfigmanager.h"

MusicPLSConfigManager::MusicPLSConfigManager()
    : MusicPlaylistRenderer()
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

    const QRegExp regx1("^File(\\d+)=(.+)");
    const QRegExp regx2("^Length(\\d+)=(-{0,1}\\d+)");

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
                item.m_songs.back().setPlayTime(TTKTime::formatDuration(regx2.cap(2).toInt() * MT_S2MS));
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

bool MusicPLSConfigManager::writeBuffer(const MusicSongItemList &items)
{
    if(items.isEmpty())
    {
        return false;
    }

    QStringList data;
    data << QString("[playlist]");

    int count = 0;
    for(int i = 0; i < items.count(); ++i)
    {
        const MusicSongItem &item = items[i];
        for(const MusicSong &song : qAsConst(item.m_songs))
        {
            ++count;
            data << QString("File%1=%2").arg(count).arg(song.path());
            data << QString("Title%1=%2").arg(count).arg(song.name());
            data << QString("Length%1=%2").arg(count).arg(TTKTime::formatDuration(song.playTime()) / 1000);
        }
    }

    data << "NumberOfEntries=" + QString::number(count);
    data << "Version=2";

    m_file.write(data.join("\n").toUtf8());
    m_file.close();
    return true;
}
