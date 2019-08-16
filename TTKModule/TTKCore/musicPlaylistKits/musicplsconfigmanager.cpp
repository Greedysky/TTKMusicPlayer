#include "musicplsconfigmanager.h"
#include "musictime.h"

MusicPLSConfigManager::MusicPLSConfigManager()
    : MusicPlaylistInterface()
{

}

bool MusicPLSConfigManager::readConfig(const QString &name)
{
    m_file.setFileName(name);
    return m_file.open(QFile::ReadOnly);
}

void MusicPLSConfigManager::readPlaylistData(MusicSongItems &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file.fileName()).baseName();

    QStringList data(QString(m_file.readAll()).split("\n"));

    if(data.isEmpty())
    {
        return;
    }

    if(!data.takeFirst().toLower().contains("[playlist]"))
    {
        return;
    }

    QRegExp fileRegExp("^File(\\d+)=(.+)");
    QRegExp lengthRegExp("^Length(\\d+)=(-{0,1}\\d+)");

    int number = 0;
    bool error = false;

    foreach(QString line, data)
    {
        if(fileRegExp.indexIn(line) > -1)
        {
            if((number = fileRegExp.cap(1).toInt()) > 0)
            {
                item.m_songs << MusicSong(fileRegExp.cap(2), 0, QString(), QString());
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
                item.m_songs.last().setMusicPlayTime(MusicTime::msecTime2LabelJustified(lengthRegExp.cap(2).toInt() * 1000));
            }
            else
            {
                error = true;
            }
        }

        if(error)
        {
            M_LOGGER_ERROR("read pls format playlist error!");
            break;
        }
    }
    m_file.close();

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
}

void MusicPLSConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    if(items.isEmpty())
    {
        return;
    }

    const MusicSongItem &item = items.first();
    QStringList data;
    data << QString("[playlist]");

    int count = 1;
    foreach(const MusicSong &song, item.m_songs)
    {
        data << QString("File%1=%2").arg(count).arg(song.getMusicPath());
        data << QString("Title%1=%2").arg(count).arg(song.getMusicName());
        data << QString("Length%1=%2").arg(count).arg(MusicTime::MusicTime::labelJustified2MsecTime(song.getMusicPlayTime())/1000);
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
}
