#include "ttkmusicconfigmanager.h"

TTKMusicConfigManager::TTKMusicConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void TTKMusicConfigManager::writeXMLConfig()
{

}

void TTKMusicConfigManager::writeMusicSongsConfig(const MusicSongItems &musics, const QString &path)
{
    if( musics.isEmpty() )
    {
        return;
    }
    //Open wirte file
    if( !writeConfig( path ) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("TTKMusicPlayer");
    for(int i=0; i<musics.count(); ++i)
    {
        QDomElement pathDom = writeDomElementMutil(musicPlayer, "musicList", QStringList() << "name" << "index" << "count",
                                  QVariantList() << musics[i].m_itemName << i << musics[i].m_songs.count());
        foreach(const MusicSong &song, musics[i].m_songs)
        {
            writeDomElementMutilText(pathDom, "value", QStringList() << "name" << "playCount" << "time",
                                     QVariantList() << song.getMusicName() << song.getMusicPlayCount()
                                                       << song.getMusicTime(), song.getMusicPath());
        }
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void TTKMusicConfigManager::readMusicSongsConfig(MusicSongItems &musics)
{
    QDomNodeList nodes = m_ddom->elementsByTagName("musicList");
    for(int i=0; i<nodes.count(); ++i)
    {
        QDomNode node = nodes.at(i);
        MusicSongItem item;
        item.m_songs = readMusicFilePath(node);

        QDomElement element = node.toElement();
        item.m_itemIndex = element.attribute("index").toInt();
        item.m_itemName = element.attribute("name");
        musics << item;
    }
}

MusicSongs TTKMusicConfigManager::readMusicFilePath(const QDomNode &node) const
{
    QDomNodeList nodelist = node.childNodes();

    MusicSongs songs;
    for(int i=0; i<nodelist.count(); i++)
    {
        QDomElement element = nodelist.at(i).toElement();
        songs << MusicSong(element.text(),
                           element.attribute("playCount").toInt(),
                           element.attribute("time"),
                           element.attribute("name"));
    }
    return songs;
}
