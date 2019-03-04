#include "musicxspfconfigmanager.h"
#include "musicplaylistmanager.h"

MusicXSPFConfigManager::MusicXSPFConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
    , MusicPlaylistInterface()
{

}

void MusicXSPFConfigManager::readPlaylistConfig(MusicSongItems &musics)
{
    bool state = false;
    const QDomNodeList &nodes = m_document->elementsByTagName("playlist");
    for(int i=0; i<nodes.count(); ++i)
    {
        const QDomNodeList &nodelist = nodes.at(i).childNodes();
        for(int i=0; i<nodelist.count(); ++i)
        {
            const QDomNode &node = nodelist.at(i);
            if(node.nodeName() == "creator")
            {
                const QDomElement &element = node.toElement();
                state = element.text().contains(APP_NAME);
                break;
            }
        }
    }

    if(!state)
    {
        MusicPlaylistManager::messageAlert();
        return;
    }

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
        musics << item;
    }
}

void MusicXSPFConfigManager::writePlaylistConfig(const MusicSongItems &musics, const QString &path)
{
    //Open wirte file
    if(musics.isEmpty() || !writeConfig(path))
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    ///////////////////////////////////////////////////////
    QDomElement musicPlayerDom = createRoot("playlist");
    //Class A
    writeDomText(musicPlayerDom, "creator", APP_NAME);
    for(int i=0; i<musics.count(); ++i)
    {
        const MusicSongItem &item = musics[i];
        //Class A
        QDomElement trackListDom = writeDomElementMutil(musicPlayerDom, "trackList", MusicXmlAttributes()
                                                       << MusicXmlAttribute("name", item.m_itemName) << MusicXmlAttribute("index", i)
                                                       << MusicXmlAttribute("count", item.m_songs.count())
                                                       << MusicXmlAttribute("sortIndex", item.m_sort.m_index)
                                                       << MusicXmlAttribute("sortType", item.m_sort.m_sortType));

        foreach(const MusicSong &song, musics[i].m_songs)
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

    //Write to file
    QTextStream out(m_file);
    m_document->save(out, 4);
}

MusicSongs MusicXSPFConfigManager::readMusicFilePath(const QDomNode &node) const
{
    const QDomNodeList &nodelist = node.childNodes();

    MusicSongs songs;
    for(int i=0; i<nodelist.count(); i++)
    {
        const QDomElement &element = nodelist.at(i).toElement();
        songs << MusicSong(element.attribute("src"),
                           element.attribute("playCount").toInt(),
                           element.attribute("time"),
                           element.attribute("name"));
    }
    return songs;
}
