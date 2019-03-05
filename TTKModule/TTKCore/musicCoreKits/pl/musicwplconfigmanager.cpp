#include "musicwplconfigmanager.h"
#include "musicplaylistmanager.h"
#include "musicversion.h"

MusicWPLConfigManager::MusicWPLConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
    , MusicPlaylistInterface()
{

}

void MusicWPLConfigManager::readPlaylistData(MusicSongItems &musics)
{
    bool state = false;
    const QDomNodeList &nodes = m_document->elementsByTagName("head");
    for(int i=0; i<nodes.count(); ++i)
    {
        const QDomNodeList &nodelist = nodes.at(i).childNodes();
        for(int i=0; i<nodelist.count(); ++i)
        {
            const QDomElement &element = nodelist.at(i).toElement();
            state = element.attribute("content").contains(APP_NAME);
            break;
        }
    }

    if(!state)
    {
        MusicPlaylistManager::messageAlert();
        return;
    }

    const QDomNodeList &sepNodes = m_document->elementsByTagName("seq");
    for(int i=0; i<sepNodes.count(); ++i)
    {
        const QDomNode &node = sepNodes.at(i);
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

void MusicWPLConfigManager::writePlaylistData(const MusicSongItems &musics, const QString &path)
{
    //Open wirte file
    if(musics.isEmpty() || !writeConfig(path))
    {
        return;
    }
    ///////////////////////////////////////////////////////
    const QDomNode &node = m_document->createProcessingInstruction(WPL_FILE_PREFIX, "version='1.0' encoding='UTF-8'");
    m_document->appendChild( node );
    ///////////////////////////////////////////////////////
    QDomElement musicPlayerDom = createRoot("smil");
    //Class A
    QDomElement headSettingDom = writeDom(musicPlayerDom, "head");
    QDomElement bodySettingDom = writeDom(musicPlayerDom, "body");
    //Class B
    writeDomElementMutil(headSettingDom, "meta", MusicXmlAttributes() << MusicXmlAttribute("name", "Generator") <<
                         MusicXmlAttribute("content", QString("%1 %2").arg(APP_NAME).arg(TTKMUSIC_VERSION_STR)));
//    writeDomElementMutil(headSettingDom, "meta", MusicXmlAttributes() << MusicXmlAttribute("name", "ItemCount") <<
//                         MusicXmlAttribute("content", QString("%1").arg(musics.count())));
    for(int i=0; i<musics.count(); ++i)
    {
        const MusicSongItem &item = musics[i];
        //Class C
        QDomElement seqDom = writeDomElementMutil(bodySettingDom, "seq", MusicXmlAttributes()
                                              << MusicXmlAttribute("name", item.m_itemName) << MusicXmlAttribute("index", i)
                                              << MusicXmlAttribute("count", item.m_songs.count())
                                              << MusicXmlAttribute("sortIndex", item.m_sort.m_index)
                                              << MusicXmlAttribute("sortType", item.m_sort.m_sortType));
        foreach(const MusicSong &song, musics[i].m_songs)
        {
            writeDomElementMutil(seqDom, "media", MusicXmlAttributes() << MusicXmlAttribute("name", song.getMusicName())
                                 << MusicXmlAttribute("playCount", song.getMusicPlayCount())
                                 << MusicXmlAttribute("time", song.getMusicPlayTime())
                                 << MusicXmlAttribute("src", song.getMusicPath()));
        }
    }

    //Write to file
    QTextStream out(m_file);
    m_document->save(out, 4);
}

MusicSongs MusicWPLConfigManager::readMusicFilePath(const QDomNode &node) const
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
