#include "musictkplconfigmanager.h"

MusicTKPLConfigManager::MusicTKPLConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
    , MusicPlaylistInterface()
{

}

bool MusicTKPLConfigManager::readPlaylistData(MusicSongItems &items)
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
        item.m_sort.m_sortType = TTKStatic_cast(Qt::SortOrder, element.attribute("sortType").toInt());
        items << item;
    }
    return true;
}

bool MusicTKPLConfigManager::writePlaylistData(const MusicSongItems &items)
{
    return writePlaylistData(items, MUSICPATH_FULL);
}

bool MusicTKPLConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    if(items.isEmpty() || !writeConfig(path))
    {
        return false;
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
        for(const MusicSong &song : qAsConst(item.m_songs))
        {
            writeDomElementMutilText(pathDom, "value", MusicXmlAttributes()
                                     << MusicXmlAttribute("name", song.getMusicName())
                                     << MusicXmlAttribute("playCount", song.getMusicPlayCount())
                                     << MusicXmlAttribute("time", song.getMusicPlayTime()), song.getMusicPath());
        }
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
    return true;
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
