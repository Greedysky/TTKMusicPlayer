#include "musicxspfconfigmanager.h"

MusicXSPFConfigManager::MusicXSPFConfigManager()
    : MusicAbstractXml(nullptr)
    , MusicPlaylistInterface()
{

}

bool MusicXSPFConfigManager::readBuffer(MusicSongItemList &items)
{
    MusicXmlNodeHelper helper(m_document->documentElement());
    helper.load();

    const QDomNodeList &trackNodes = m_document->elementsByTagName(helper.nodeName("trackList"));
    for(int i = 0; i < trackNodes.count(); ++i)
    {
        const QDomNode &node = trackNodes.at(i);
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

bool MusicXSPFConfigManager::writeBuffer(const MusicSongItemList &items, const QString &path)
{
    if(items.isEmpty() || !toFile(path))
    {
        return false;
    }

    createProcessingInstruction();
    QDomElement rootDom = createRoot("playlist", {{"version", "1"},
                                                  {"xmlns", "http://xspf.org/ns/0/"}});
    writeDomText(rootDom, "creator", APP_NAME);

    for(int i = 0; i < items.count(); ++i)
    {
        const MusicSongItem &item = items[i];
        QDomElement trackListDom = writeDomMutilElement(rootDom, "trackList", {
                                                       {"name", item.m_itemName},
                                                       {"index", i},
                                                       {"count", item.m_songs.count()},
                                                       {"sortIndex", item.m_sort.m_type},
                                                       {"sortType", item.m_sort.m_order}});

        for(const MusicSong &song : qAsConst(items[i].m_songs))
        {
            QDomElement trackDom = writeDomMutilElement(trackListDom, "track", {
                                                        {"name", song.musicName()},
                                                        {"playCount", song.musicPlayCount()},
                                                        {"time", song.musicPlayTime()},
                                                        {"src", song.musicPath()}});
            writeDomText(trackDom, "location", song.musicPath());
            writeDomText(trackDom, "title", song.musicArtistBack());
            writeDomText(trackDom, "creator", song.musicArtistFront());
            writeDomText(trackDom, "annotation", QString());
            writeDomText(trackDom, "album", QString());
            writeDomText(trackDom, "trackNum", QString());
            writeDomElementText(trackDom, "meta", {"rel", "year"}, QString());
        }
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
    return true;
}

MusicSongList MusicXSPFConfigManager::readMusicFilePath(const QDomNode &node) const
{
    const QDomNodeList &nodeList = node.childNodes();

    MusicSongList songs;
    for(int i = 0; i < nodeList.count(); ++i)
    {
        const QDomElement &element = nodeList.at(i).toElement();
        MusicSong song(element.attribute("src"), element.attribute("time"), element.attribute("name"));
        song.setMusicPlayCount(element.attribute("playCount").toInt());
        songs << song;
    }
    return songs;
}
