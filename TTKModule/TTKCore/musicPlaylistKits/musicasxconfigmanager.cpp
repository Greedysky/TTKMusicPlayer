#include "musicasxconfigmanager.h"

MusicASXConfigManager::MusicASXConfigManager()
    : MusicAbstractXml(nullptr)
    , MusicPlaylistInterface()
{

}

bool MusicASXConfigManager::readBuffer(MusicSongItemList &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file->fileName()).baseName();

    MusicXmlNodeHelper helper(m_document->documentElement());
    helper.load();

    const QDomNodeList &itemNodes = m_document->elementsByTagName(helper.nodeName("Entry"));
    for(int i = 0; i < itemNodes.count(); ++i)
    {
        const QDomNode &node = itemNodes.at(i);
        const QDomNodeList &paramNodes = node.childNodes();

        QString duration, path;
        for(int j = 0; j < paramNodes.count(); ++j)
        {
            const QDomNode &paramNode = paramNodes.at(j);
            if(paramNode.nodeName().toLower() == "duration")
            {
                duration = paramNode.toElement().attribute("value");
                duration = duration.mid(3, 5);
            }
            else if(paramNode.nodeName().toLower() == "ref")
            {
                path = paramNode.toElement().attribute("href");
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

bool MusicASXConfigManager::writeBuffer(const MusicSongItemList &items, const QString &path)
{
    if(items.isEmpty() || !toFile(path))
    {
        return false;
    }

    QDomElement rootDom = createRoot("Asx", {"version ", "3.0"});

    for(int i = 0; i < items.count(); ++i)
    {
        const MusicSongItem &item = items[i];
        writeDomText(rootDom, "Title", item.m_itemName);

        for(const MusicSong &song : qAsConst(items[i].m_songs))
        {
            QDomElement trackDom = writeDomNode(rootDom, "Entry");

            writeDomText(trackDom, "Title", song.musicArtistBack());
            writeDomElement(trackDom, "Ref", {"href", song.musicPath()});
            writeDomElement(trackDom, "Duration", {"value", "00:" + song.musicPlayTime() + ".000"});
            writeDomText(trackDom, "Author", APP_NAME);
        }
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
    return true;
}
