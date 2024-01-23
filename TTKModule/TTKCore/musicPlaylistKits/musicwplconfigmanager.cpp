#include "musicwplconfigmanager.h"
#include "ttkversion.h"

MusicWPLConfigManager::MusicWPLConfigManager()
    : TTKAbstractXml()
    , MusicPlaylistInterface()
{

}

bool MusicWPLConfigManager::readBuffer(MusicSongItemList &items)
{
    TTKXmlHelper helper(m_document->documentElement());
    helper.load();

    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file->fileName()).baseName();

    const QDomNodeList &nodes = m_document->elementsByTagName(helper.nodeName("seq"));
    for(int i = 0; i < nodes.count(); ++i)
    {
        const QDomNode &node = nodes.item(i);
        item.m_songs << readMusicFilePath(node);
    }

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
    return true;
}

bool MusicWPLConfigManager::writeBuffer(const MusicSongItemList &items)
{
    if(items.isEmpty())
    {
        return false;
    }

    const QDomNode &node = m_document->createProcessingInstruction(WPL_FILE_SUFFIX, "version='1.0' encoding='UTF-8'");
    m_document->appendChild(node);

    QDomElement rootDom = createRoot("smil");
    QDomElement headDom = writeDomElement(rootDom, "head");
    QDomElement bodyDom = writeDomElement(rootDom, "body");

    writeDomMultiElement(headDom, "meta", {{"name", "Generator"}, {"content", QString("%1 %2").arg(TTK_APP_NAME, TTK_VERSION_STR)}});

    for(int i = 0; i < items.count(); ++i)
    {
        const MusicSongItem &item = items[i];
        QDomElement seqDom = writeDomElement(bodyDom, "seq");

        for(const MusicSong &song : qAsConst(item.m_songs))
        {
            writeDomElement(seqDom, "media", {"src", song.path()});
        }
    }

    save();
    return true;
}

MusicSongList MusicWPLConfigManager::readMusicFilePath(const QDomNode &node) const
{
    const QDomNodeList &nodes = node.childNodes();

    MusicSongList songs;
    for(int i = 0; i < nodes.count(); ++i)
    {
        const QDomElement &element = nodes.item(i).toElement();
        songs << MusicSong(element.attribute("src"));
    }
    return songs;
}
