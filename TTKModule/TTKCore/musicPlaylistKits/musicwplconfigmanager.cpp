#include "musicwplconfigmanager.h"
#include "ttkversion.h"

MusicWPLConfigManager::MusicWPLConfigManager()
    : MusicAbstractXml(nullptr)
    , MusicPlaylistInterface()
{

}

bool MusicWPLConfigManager::readBuffer(MusicSongItemList &items)
{
    MusicXmlNodeHelper helper(m_document->documentElement());
    helper.load();

    const QDomNodeList &sepNodes = m_document->elementsByTagName(helper.nodeName("seq"));
    for(int i = 0; i < sepNodes.count(); ++i)
    {
        const QDomNode &node = sepNodes.at(i);
        MusicSongItem item;
        item.m_itemName = QFileInfo(m_file->fileName()).baseName();
        item.m_songs = readMusicFilePath(node);
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

    const QDomNode &node = m_document->createProcessingInstruction(WPL_FILE_PREFIX, "version='1.0' encoding='UTF-8'");
    m_document->appendChild(node);

    QDomElement rootDom = createRoot("smil");
    QDomElement headDom = writeDomNode(rootDom, "head");
    QDomElement bodyDom = writeDomNode(rootDom, "body");

    writeDomMutilElement(headDom, "meta", {{"name", "Generator"},
                                           {"content", QString("%1 %2").arg(APP_NAME, TTK_VERSION_STR)}});
    for(int i = 0; i < items.count(); ++i)
    {
        const MusicSongItem &item = items[i];
        QDomElement seqDom = writeDomNode(bodyDom, "seq");

        for(const MusicSong &song : qAsConst(item.m_songs))
        {
            writeDomElement(seqDom, "media", {"src", song.musicPath()});
        }
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
    return true;
}

MusicSongList MusicWPLConfigManager::readMusicFilePath(const QDomNode &node) const
{
    const QDomNodeList &nodeList = node.childNodes();

    MusicSongList songs;
    for(int i = 0; i < nodeList.count(); ++i)
    {
        const QDomElement &element = nodeList.at(i).toElement();
        songs << MusicSong(element.attribute("src"));
    }
    return songs;
}
