#include "musictkplconfigmanager.h"

MusicTKPLConfigManager::MusicTKPLConfigManager()
    : MusicAbstractXml(nullptr)
    , MusicPlaylistInterface()
{

}

bool MusicTKPLConfigManager::readPlaylistData(MusicSongItemList &items)
{
    const QDomNodeList &nodes = m_document->elementsByTagName("musicList");
    for(int i = 0; i < nodes.count(); ++i)
    {
        const QDomNode &node = nodes.at(i);
        MusicSongItem item;
        item.m_songs = readMusicFilePath(node);

        const QDomElement &element = node.toElement();
        item.m_itemIndex = element.attribute("index").toInt();
        item.m_itemName = element.attribute("name");

        const QString &string = element.attribute("sortIndex");
        item.m_sort.m_type = string.isEmpty() ? -1 : string.toInt();
        item.m_sort.m_order = TTKStatic_cast(Qt::SortOrder, element.attribute("sortType").toInt());
        items << item;
    }
    return true;
}

bool MusicTKPLConfigManager::writePlaylistData(const MusicSongItemList &items, const QString &path)
{
    if(items.isEmpty() || !writeConfig(path))
    {
        return false;
    }
    //
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot(APP_NAME);
    for(int i = 0; i < items.count(); ++i)
    {
        const MusicSongItem &item = items[i];
        QDomElement pathDom = writeDomElementMutil(musicPlayer, "musicList", MusicXmlAttributeList()
                                                   << MusicXmlAttribute("name", item.m_itemName)
                                                   << MusicXmlAttribute("index", i)
                                                   << MusicXmlAttribute("count", item.m_songs.count())
                                                   << MusicXmlAttribute("sortIndex", item.m_sort.m_type)
                                                   << MusicXmlAttribute("sortType", item.m_sort.m_order));
        for(const MusicSong &song : qAsConst(item.m_songs))
        {
            QString playTime = song.musicPlayTime();
            if(item.m_itemIndex == MUSIC_NETWORK_LIST && playTime == TTK_DEFAULT_STR)
            {
                playTime = MusicObject::generateMusicPlayTime(song.musicPath());
            }

            writeDomElementMutilText(pathDom, "value", MusicXmlAttributeList()
                                     << MusicXmlAttribute("name", song.musicName())
                                     << MusicXmlAttribute("playCount", song.musicPlayCount())
                                     << MusicXmlAttribute("time", playTime), song.musicPath());
        }
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
    return true;
}

MusicSongList MusicTKPLConfigManager::readMusicFilePath(const QDomNode &node) const
{
    const QDomNodeList &nodeList = node.childNodes();

    MusicSongList songs;
    for(int i = 0; i < nodeList.count(); ++i)
    {
        const QDomElement &element = nodeList.at(i).toElement();
        MusicSong song(element.text(), element.attribute("time"), element.attribute("name"), true);
        song.setMusicPlayCount(element.attribute("playCount").toInt());
        songs << song;
    }
    return songs;
}
