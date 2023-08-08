#include "musicwplconfigmanager.h"
#include "ttkversion.h"

MusicWPLConfigManager::MusicWPLConfigManager()
    : TTKXmlDocument(nullptr)
    , MusicPlaylistInterface()
{

}

bool MusicWPLConfigManager::readBuffer(MusicSongItemList &items)
{
    TTKXmlNodeHelper helper(m_document->documentElement());
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

bool MusicWPLConfigManager::writeBuffer(const MusicSongItemList &items, const QString &path)
{
    if(items.isEmpty() || !toFile(path))
    {
        return false;
    }

    const QDomNode &node = m_document->createProcessingInstruction(WPL_FILE_SUFFIX, "version='1.0' encoding='UTF-8'");
    m_document->appendChild(node);

    QDomElement rootDom = createRoot("smil");
    QDomElement headDom = writeDomNode(rootDom, "head");
    QDomElement bodyDom = writeDomNode(rootDom, "body");

    writeDomMutilElement(headDom, "meta", {{"name", "Generator"},
                                           {"content", QString("%1 %2").arg(TTK_APP_NAME, TTK_VERSION_STR)}});
    for(int i = 0; i < items.count(); ++i)
    {
        const MusicSongItem &item = items[i];
        QDomElement seqDom = writeDomNode(bodyDom, "seq");

        for(const MusicSong &song : qAsConst(item.m_songs))
        {
            writeDomElement(seqDom, "media", {"src", song.path()});
        }
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
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
