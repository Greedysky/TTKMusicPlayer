#include "musicxspfconfigmanager.h"

MusicXSPFConfigManager::MusicXSPFConfigManager()
    : TTKAbstractXml()
    , MusicPlaylistInterface()
{

}

bool MusicXSPFConfigManager::readBuffer(MusicSongItemList &items)
{
    TTKXmlNodeHelper helper(m_document->documentElement());
    helper.load();

    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file->fileName()).baseName();

    const QDomNodeList &nodes = m_document->elementsByTagName(helper.nodeName("trackList"));
    for(int i = 0; i < nodes.count(); ++i)
    {
        const QDomNodeList &trackNodes = nodes.item(i).childNodes();
        for(int i = 0; i < trackNodes.count(); ++i)
        {
            const QDomNodeList &trackChildNodes = trackNodes.item(i).childNodes();

            QString duration, path;
            for(int i = 0; i < trackChildNodes.count(); ++i)
            {
                const QDomElement &element = trackChildNodes.item(i).toElement();
                const QString &name = element.nodeName().toLower();

                if(name == "location")
                {
                    path = element.text();
                    path.remove("file://");
                }
                else if(name == "length" || name == "duration")
                {
                    duration = element.text();
                }
            }

            if(!path.isEmpty())
            {
                item.m_songs << MusicSong(path, duration);
            }
        }
    }

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
    return true;
}

bool MusicXSPFConfigManager::writeBuffer(const MusicSongItemList &items)
{
    if(items.isEmpty())
    {
        return false;
    }

    createProcessingInstruction();
    QDomElement rootDom = createRoot("playlist", {{"version", "1"}, {"xmlns", "http://xspf.org/ns/0/"}});
    writeDomElement(rootDom, "creator", TTK_APP_NAME);

    for(int i = 0; i < items.count(); ++i)
    {
        QDomElement trackListDom = writeDomElement(rootDom, "trackList");
        for(const MusicSong &song : qAsConst(items[i].m_songs))
        {
            QDomElement trackDom = writeDomElement(trackListDom, "track");

            writeDomElement(trackDom, "location", song.path());
            writeDomElement(trackDom, "title", song.title());
            writeDomElement(trackDom, "creator", song.artist());
            writeDomElement(trackDom, "duration", song.playTime());
            writeDomElement(trackDom, "annotation", QString());
            writeDomElement(trackDom, "album", QString());
            writeDomElement(trackDom, "trackNum", QString());
            writeDomElement(trackDom, "meta", {"rel", "year"}, {});
        }
    }

    save();
    return true;
}
