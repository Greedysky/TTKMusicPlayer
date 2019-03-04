#include "musicasxconfigmanager.h"
#include "musicplaylistmanager.h"

MusicASXConfigManager::MusicASXConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
    , MusicPlaylistInterface()
{

}

void MusicASXConfigManager::readPlaylistConfig(MusicSongItems &musics)
{
    bool state = false;
    const QDomNodeList &nodes = m_document->elementsByTagName("author");
    for(int i=0; i<nodes.count(); ++i)
    {
        const QDomNode &node = nodes.at(i);
        if(node.nodeName() == "author")
        {
            const QDomElement &element = node.toElement();
            state = element.text().contains(APP_NAME);
            break;
        }
    }

    if(!state)
    {
        MusicPlaylistManager::messageAlert();
        return;
    }

    MusicSongItem item;
    const QDomNodeList &itemNodes = m_document->elementsByTagName("ttkitem");
    for(int i=0; i<itemNodes.count(); ++i)
    {
        const QDomElement &element = itemNodes.at(i).toElement();
        item.m_songs << MusicSong(element.attribute("src"),
                                  element.attribute("playCount").toInt(),
                                  element.attribute("time"),
                                  element.attribute("name"));
    }

    const QDomNodeList &listNodes = m_document->elementsByTagName("ttklist");
    if(!listNodes.isEmpty())
    {
        const QDomElement &element = listNodes.at(0).toElement();
        item.m_itemIndex = element.attribute("index").toInt();
        item.m_itemName = element.attribute("name");

        const QString &string = element.attribute("sortIndex");
        item.m_sort.m_index = string.isEmpty() ? -1 : string.toInt();
        item.m_sort.m_sortType = MStatic_cast(Qt::SortOrder, element.attribute("sortType").toInt());
        musics << item;
    }
}

void MusicASXConfigManager::writePlaylistConfig(const MusicSongItems &musics, const QString &path)
{
    //Open wirte file
    if(musics.isEmpty() || !writeConfig(path))
    {
        return;
    }
    ///////////////////////////////////////////////////////
    QDomElement musicPlayerDom = createRoot(ASX_FILE_PREFIX, MusicXmlAttribute("version ", "3.0"));
    //Class A
    for(int i=0; i<musics.count(); ++i)
    {
        const MusicSongItem &item = musics[i];

        writeDomText(musicPlayerDom, "title", item.m_itemName);

        foreach(const MusicSong &song, musics[i].m_songs)
        {
            //Class B
            QDomElement trackDom = writeDom(musicPlayerDom, "entry");

            writeDomText(trackDom, "title", song.getMusicArtistBack());
            writeDomElement(trackDom, "ref", MusicXmlAttribute("href", song.getMusicPath()));

            writeDomText(trackDom, "author", APP_NAME);
            writeDomElementMutil(trackDom, "ttkitem", MusicXmlAttributes()
                                 << MusicXmlAttribute("name", song.getMusicName())
                                 << MusicXmlAttribute("playCount", song.getMusicPlayCount())
                                 << MusicXmlAttribute("time", song.getMusicPlayTime())
                                 << MusicXmlAttribute("src", song.getMusicPath()));

            writeDomElementMutil(trackDom, "ttklist", MusicXmlAttributes()
                                 << MusicXmlAttribute("name", item.m_itemName) << MusicXmlAttribute("index", i)
                                 << MusicXmlAttribute("count", item.m_songs.count())
                                 << MusicXmlAttribute("sortIndex", item.m_sort.m_index)
                                 << MusicXmlAttribute("sortType", item.m_sort.m_sortType));
        }
    }

    //Write to file
    QTextStream out(m_file);
    m_document->save(out, 4);
}
