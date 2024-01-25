#include "musicasxconfigmanager.h"

MusicASXConfigManager::MusicASXConfigManager()
    : TTKAbstractXml()
    , MusicPlaylistInterface()
{

}

bool MusicASXConfigManager::readBuffer(MusicSongItemList &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file->fileName()).baseName();

    TTKXmlHelper helper(m_document->documentElement());
    helper.load();

    const QDomNodeList &nodes = m_document->elementsByTagName(helper.nodeName("Entry"));
    for(int i = 0; i < nodes.count(); ++i)
    {
        const QDomNodeList &paramNodes = nodes.item(i).childNodes();

        QString duration, path;
        for(int j = 0; j < paramNodes.count(); ++j)
        {
            const QDomNode &paramNode = paramNodes.item(j);
            const QDomElement &element = paramNode.toElement();
            const QString &name = paramNode.nodeName().toLower();

            if(name == "duration" || name == "length")
            {
                duration = element.attribute("value");
                duration = duration.mid(3, 5);
            }
            else if(name == "ref")
            {
                path = element.attribute("href");
            }
        }

        if(!path.isEmpty())
        {
            item.m_songs << MusicSong(path, duration);
        }
    }

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
    return true;
}

bool MusicASXConfigManager::writeBuffer(const MusicSongItemList &items)
{
    if(items.isEmpty())
    {
        return false;
    }

    QDomElement rootDom = createRoot("Asx", TTKXmlAttr("version ", "3.0"));

    for(int i = 0; i < items.count(); ++i)
    {
        const MusicSongItem &item = items[i];
        writeDomElement(rootDom, "Title", item.m_itemName);

        for(const MusicSong &song : qAsConst(items[i].m_songs))
        {
            QDomElement trackDom = writeDomElement(rootDom, "Entry");

            writeDomElement(trackDom, "Title", song.title());
            writeDomElement(trackDom, "Ref", {"href", song.path()});
            writeDomElement(trackDom, "Duration", {"value", "00:" + song.duration() + ".000"});
            writeDomElement(trackDom, "Author", TTK_APP_NAME);
        }
    }

    save();
    return true;
}
