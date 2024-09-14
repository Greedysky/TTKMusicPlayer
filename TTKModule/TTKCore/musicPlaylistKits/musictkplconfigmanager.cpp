#include "musictkplconfigmanager.h"

MusicTKPLConfigManager::MusicTKPLConfigManager()
    : TTKAbstractXml()
    , MusicPlaylistInterface()
{

}

bool MusicTKPLConfigManager::readBuffer(MusicSongItemList &items)
{
    const QDomNodeList &nodes = m_document->elementsByTagName("musicList");
    for(int i = 0; i < nodes.count(); ++i)
    {
        const QDomNode &node = nodes.item(i);
        MusicSongItem item;
        item.m_songs = readMusicFilePath(node);

        const QDomElement &element = node.toElement();
        item.m_itemIndex = element.attribute("index").toInt();
        item.m_itemName = element.attribute("name");

        bool ok = false;
        item.m_id = element.attribute("id").toInt(&ok);
        if(!ok)
        {
            item.m_id = item.m_itemIndex;
        }

        const QString &string = element.attribute("sortIndex");
        item.m_sort.m_type = string.isEmpty() ? -1 : string.toInt();
        item.m_sort.m_order = TTKStaticCast(Qt::SortOrder, element.attribute("sortType").toInt());
        items << item;
    }
    return true;
}

bool MusicTKPLConfigManager::writeBuffer(const MusicSongItemList &items)
{
    if(items.isEmpty())
    {
        return false;
    }

    createProcessingInstruction();
    QDomElement rootDom = createRoot(TTK_APP_NAME);

    for(int i = 0; i < items.count(); ++i)
    {
        const MusicSongItem &item = items[i];
        QDomElement pathDom = writeDomMultiElement(rootDom, "musicList", {{"index", i},
                                                                          {"id", item.m_id},
                                                                          {"name", item.m_itemName},
                                                                          {"count", item.m_songs.count()},
                                                                          {"sortIndex", item.m_sort.m_type},
                                                                          {"sortType", item.m_sort.m_order}});
        for(const MusicSong &song : qAsConst(item.m_songs))
        {
            QString duration = song.duration();
            if(item.m_itemIndex == MUSIC_NETWORK_LIST && duration == TTK_DEFAULT_STR)
            {
                duration = TTK::generateNetworkSongTime(song.path());
            }

            writeDomMultiElement(pathDom, "value", {{"name", song.name()},
                                                    {"playCount", song.playCount()},
                                                    {"time", duration}}, song.path());
        }
    }

    save();
    return true;
}

MusicSongList MusicTKPLConfigManager::readMusicFilePath(const QDomNode &node) const
{
    const QDomNodeList &nodes = node.childNodes();

    MusicSongList songs;
    for(int i = 0; i < nodes.count(); ++i)
    {
        const QDomElement &element = nodes.item(i).toElement();
        MusicSong song(element.text(), element.attribute("time"), element.attribute("name"), true);
        song.setPlayCount(element.attribute("playCount").toInt());
        songs << song;
    }
    return songs;
}
