#include "musiclistconfigmanager.h"

MusicListConfigManager::MusicListConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

QString MusicListConfigManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicListConfigManager::writeMusicSongsConfig(const MusicSongItems &musics)
{
    writeMusicSongsConfig(musics, MUSICPATH_FULL);
}

void MusicListConfigManager::writeMusicSongsConfig(const MusicSongItems &musics, const QString &path)
{
    //Open wirte file
    if( musics.isEmpty() || !writeConfig( path ) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot(APPNAME);
    for(int i=0; i<musics.count(); ++i)
    {
        const MusicSongItem item = musics[i];
        QDomElement pathDom = writeDomElementMutil(musicPlayer, "musicList",
                              MusicXmlAttributes() << MusicXmlAttribute("name", item.m_itemName) << MusicXmlAttribute("index", i)
                                              << MusicXmlAttribute("count", item.m_songs.count()) << MusicXmlAttribute("sortIndex", item.m_sort.m_index)
                                              << MusicXmlAttribute("sortType", item.m_sort.m_sortType));
        foreach(const MusicSong &song, item.m_songs)
        {
            writeDomElementMutilText(pathDom, "value", MusicXmlAttributes() << MusicXmlAttribute("name", song.getMusicName())
                                     << MusicXmlAttribute("playCount", song.getMusicPlayCount())
                                     << MusicXmlAttribute("time", song.getMusicTime()), song.getMusicPath());
        }
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicListConfigManager::readMusicSongsConfig(MusicSongItems &musics)
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
        QString string = element.attribute("sortIndex");
        item.m_sort.m_index = string.isEmpty() ? -1 : string.toInt();
        item.m_sort.m_sortType = MStatic_cast(Qt::SortOrder, element.attribute("sortType").toInt());
        musics << item;
    }
}

MusicSongs MusicListConfigManager::readMusicFilePath(const QDomNode &node) const
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
