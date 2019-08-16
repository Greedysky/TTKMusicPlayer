#include "musictkplconfigmanager.h"

MusicTKPLConfigManager::MusicTKPLConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
    , MusicPlaylistInterface()
{

}

void MusicTKPLConfigManager::readPlaylistData(MusicSongItems &items)
{
    const QDomNodeList &nodes = m_document->elementsByTagName("musicList");
    for(int i=0; i<nodes.count(); ++i)
    {
        const QDomNode &node = nodes.at(i);
        MusicSongItem item;
        item.m_songs = readMusicFilePath(node);

        const QDomElement &element = node.toElement();
        item.m_itemIndex = element.attribute("index").toInt();
        item.m_itemName = element.attribute("name");

        const QString &string = element.attribute("sortIndex");
        item.m_sort.m_index = string.isEmpty() ? -1 : string.toInt();
        item.m_sort.m_sortType = MStatic_cast(Qt::SortOrder, element.attribute("sortType").toInt());
        items << item;
    }
}

void MusicTKPLConfigManager::writePlaylistData(const MusicSongItems &items)
{
    writePlaylistData(items, MUSICPATH_FULL);
}

void MusicTKPLConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    if(items.isEmpty() || !writeConfig(path))
    {
        return;
    }
    //
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot(APP_NAME);
    for(int i=0; i<items.count(); ++i)
    {
        const MusicSongItem &item = items[i];
        QDomElement pathDom = writeDomElementMutil(musicPlayer, "musicList", MusicXmlAttributes()
                              << MusicXmlAttribute("name", item.m_itemName) << MusicXmlAttribute("index", i)
                              << MusicXmlAttribute("count", item.m_songs.count()) << MusicXmlAttribute("sortIndex", item.m_sort.m_index)
                              << MusicXmlAttribute("sortType", item.m_sort.m_sortType));
        foreach(const MusicSong &song, item.m_songs)
        {
            writeDomElementMutilText(pathDom, "value", MusicXmlAttributes()
                                     << MusicXmlAttribute("name", song.getMusicName())
                                     << MusicXmlAttribute("playCount", song.getMusicPlayCount())
                                     << MusicXmlAttribute("time", song.getMusicPlayTime()), song.getMusicPath());
        }
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
}

MusicSongs MusicTKPLConfigManager::readMusicFilePath(const QDomNode &node) const
{
    const QDomNodeList &nodeList = node.childNodes();

    MusicSongs songs;
    for(int i=0; i<nodeList.count(); i++)
    {
        const QDomElement &element = nodeList.at(i).toElement();
        songs << MusicSong(element.text(),
                           element.attribute("playCount").toInt(),
                           element.attribute("time"),
                           element.attribute("name"));
    }
    return songs;
}
