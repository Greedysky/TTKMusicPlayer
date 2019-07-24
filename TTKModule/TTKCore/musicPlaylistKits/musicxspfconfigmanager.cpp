#include "musicxspfconfigmanager.h"

MusicXSPFConfigManager::MusicXSPFConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
    , MusicPlaylistInterface()
{

}

void MusicXSPFConfigManager::readPlaylistData(MusicSongItems &items)
{
    const QDomNodeList &trackNodes = m_document->elementsByTagName("trackList");
    for(int i=0; i<trackNodes.count(); ++i)
    {
        const QDomNode &node = trackNodes.at(i);
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

void MusicXSPFConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    if(items.isEmpty() || !writeConfig(path))
    {
        return;
    }
    //
    createProcessingInstruction();
    //
    QDomElement musicPlayerDom = createRoot("playlist");
    //Class A
    writeDomText(musicPlayerDom, "creator", APP_NAME);
    for(int i=0; i<items.count(); ++i)
    {
        const MusicSongItem &item = items[i];
        //Class A
        QDomElement trackListDom = writeDomElementMutil(musicPlayerDom, "trackList", MusicXmlAttributes()
                                                       << MusicXmlAttribute("name", item.m_itemName) << MusicXmlAttribute("index", i)
                                                       << MusicXmlAttribute("count", item.m_songs.count())
                                                       << MusicXmlAttribute("sortIndex", item.m_sort.m_index)
                                                       << MusicXmlAttribute("sortType", item.m_sort.m_sortType));

        foreach(const MusicSong &song, items[i].m_songs)
        {
            //Class B
            QDomElement trackDom = writeDomElementMutil(trackListDom, "track", MusicXmlAttributes()
                                                        << MusicXmlAttribute("name", song.getMusicName())
                                                        << MusicXmlAttribute("playCount", song.getMusicPlayCount())
                                                        << MusicXmlAttribute("time", song.getMusicPlayTime())
                                                        << MusicXmlAttribute("src", song.getMusicPath()));
            writeDomText(trackDom, "location", song.getMusicPath());
            writeDomText(trackDom, "title", song.getMusicArtistBack());
            writeDomText(trackDom, "creator", song.getMusicArtistFront());
            writeDomText(trackDom, "annotation", QString());
            writeDomText(trackDom, "album", QString());
            writeDomText(trackDom, "trackNum", QString());
            writeDomElementText(trackDom, "meta", MusicXmlAttribute("rel", "year"), QString());
        }
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
}

MusicSongs MusicXSPFConfigManager::readMusicFilePath(const QDomNode &node) const
{
    const QDomNodeList &nodeList = node.childNodes();

    MusicSongs songs;
    for(int i=0; i<nodeList.count(); i++)
    {
        const QDomElement &element = nodeList.at(i).toElement();
        songs << MusicSong(element.attribute("src"),
                           element.attribute("playCount").toInt(),
                           element.attribute("time"),
                           element.attribute("name"));
    }
    return songs;
}
