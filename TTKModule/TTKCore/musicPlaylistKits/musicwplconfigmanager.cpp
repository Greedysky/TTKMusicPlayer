#include "musicwplconfigmanager.h"
#include "ttkversion.h"

MusicWPLConfigManager::MusicWPLConfigManager()
    : MusicAbstractXml(nullptr)
    , MusicPlaylistInterface()
{

}

bool MusicWPLConfigManager::readPlaylistData(MusicSongItems &items)
{
    MusicXmlNodeHelper nodeHelper(m_document->documentElement());
    nodeHelper.load();

    const QDomNodeList &sepNodes = m_document->elementsByTagName(nodeHelper.nodeName("seq"));
    for(int i=0; i<sepNodes.count(); ++i)
    {
        const QDomNode &node = sepNodes.at(i);
        MusicSongItem item;
        item.m_itemName = QFileInfo(m_file->fileName()).baseName();
        item.m_songs = readMusicFilePath(node);
        items << item;
    }
    return true;
}

bool MusicWPLConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    if(items.isEmpty() || !writeConfig(path))
    {
        return false;
    }
    //
    const QDomNode &node = m_document->createProcessingInstruction(WPL_FILE_PREFIX, "version='1.0' encoding='UTF-8'");
    m_document->appendChild(node);
    //
    QDomElement musicPlayerDom = createRoot("smil");

    QDomElement headSettingDom = writeDomNode(musicPlayerDom, "head");
    QDomElement bodySettingDom = writeDomNode(musicPlayerDom, "body");

    writeDomElementMutil(headSettingDom, "meta", {MusicXmlAttribute("name", "Generator"),
                         MusicXmlAttribute("content", QString("%1 %2").arg(APP_NAME, TTKMUSIC_VERSION_STR))});
    for(int i=0; i<items.count(); ++i)
    {
        const MusicSongItem &item = items[i];
        QDomElement seqDom = writeDomNode(bodySettingDom, "seq");
        for(const MusicSong &song : qAsConst(item.m_songs))
        {
            writeDomElementMutil(seqDom, "media", {MusicXmlAttribute("src", song.musicPath())});
        }
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
    return true;
}

MusicSongs MusicWPLConfigManager::readMusicFilePath(const QDomNode &node) const
{
    const QDomNodeList &nodeList = node.childNodes();

    MusicSongs songs;
    for(int i=0; i<nodeList.count(); i++)
    {
        const QDomElement &element = nodeList.at(i).toElement();
        songs << MusicSong(element.attribute("src"));
    }
    return songs;
}
