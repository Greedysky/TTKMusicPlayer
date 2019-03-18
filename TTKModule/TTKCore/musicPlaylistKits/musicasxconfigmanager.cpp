#include "musicasxconfigmanager.h"

MusicASXConfigManager::MusicASXConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
    , MusicPlaylistInterface()
{

}

void MusicASXConfigManager::readPlaylistData(MusicSongItems &items)
{
    MusicSongItem item;
    const QDomNodeList &itemNodes = m_document->elementsByTagName("ttkItem");
    for(int i=0; i<itemNodes.count(); ++i)
    {
        const QDomElement &element = itemNodes.at(i).toElement();
        item.m_songs << MusicSong(element.attribute("src"),
                                  element.attribute("playCount").toInt(),
                                  element.attribute("time"),
                                  element.attribute("name"));
    }

    const QDomNodeList &listNodes = m_document->elementsByTagName("ttkList");
    if(!listNodes.isEmpty())
    {
        const QDomElement &element = listNodes.at(0).toElement();
        item.m_itemIndex = element.attribute("index").toInt();
        item.m_itemName = element.attribute("name");

        const QString &string = element.attribute("sortIndex");
        item.m_sort.m_index = string.isEmpty() ? -1 : string.toInt();
        item.m_sort.m_sortType = MStatic_cast(Qt::SortOrder, element.attribute("sortType").toInt());
        items << item;
    }
}

void MusicASXConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    if(items.isEmpty() || !writeConfig(path))
    {
        return;
    }
    ///////////////////////////////////////////////////////
    QDomElement musicPlayerDom = createRoot(ASX_FILE_PREFIX, MusicXmlAttribute("version ", "3.0"));
    //Class A
    for(int i=0; i<items.count(); ++i)
    {
        const MusicSongItem &item = items[i];

        writeDomText(musicPlayerDom, "title", item.m_itemName);

        foreach(const MusicSong &song, items[i].m_songs)
        {
            //Class B
            QDomElement trackDom = writeDomNode(musicPlayerDom, "entry");

            writeDomText(trackDom, "title", song.getMusicArtistBack());
            writeDomElement(trackDom, "ref", MusicXmlAttribute("href", song.getMusicPath()));

            writeDomText(trackDom, "author", APP_NAME);
            writeDomElementMutil(trackDom, "ttkItem", MusicXmlAttributes()
                                 << MusicXmlAttribute("name", song.getMusicName())
                                 << MusicXmlAttribute("playCount", song.getMusicPlayCount())
                                 << MusicXmlAttribute("time", song.getMusicPlayTime())
                                 << MusicXmlAttribute("src", song.getMusicPath()));

            writeDomElementMutil(trackDom, "ttkList", MusicXmlAttributes()
                                 << MusicXmlAttribute("name", item.m_itemName) << MusicXmlAttribute("index", i)
                                 << MusicXmlAttribute("count", item.m_songs.count())
                                 << MusicXmlAttribute("sortIndex", item.m_sort.m_index)
                                 << MusicXmlAttribute("sortType", item.m_sort.m_sortType));
        }
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
}
