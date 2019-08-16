#include "musicasxconfigmanager.h"

MusicASXConfigManager::MusicASXConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
    , MusicPlaylistInterface()
{

}

void MusicASXConfigManager::readPlaylistData(MusicSongItems &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file->fileName()).baseName();

    const QDomNodeList &itemNodes = m_document->elementsByTagName("Entry");
    for(int i=0; i<itemNodes.count(); ++i)
    {
        const QDomNode &node = itemNodes.at(i);
        const QDomNodeList &paramNodes = node.childNodes();

        QString duration, path;
        for(int j=0; j<paramNodes.count(); ++j)
        {
            const QDomNode &paramNode = paramNodes.at(j);
            if(paramNode.nodeName() == "Duration")
            {
                duration = paramNode.toElement().attribute("value");
                duration = duration.mid(3, 5);
            }
            else if(paramNode.nodeName() == "Ref")
            {
                path = paramNode.toElement().attribute("href");
            }
        }

        if(!path.isEmpty())
        {
            item.m_songs << MusicSong(path, 0, duration, QString());
        }
    }

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
}

void MusicASXConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    if(items.isEmpty() || !writeConfig(path))
    {
        return;
    }
    //
    QDomElement musicPlayerDom = createRoot("Asx", MusicXmlAttribute("version ", "3.0"));
    //Class A
    for(int i=0; i<items.count(); ++i)
    {
        const MusicSongItem &item = items[i];

        writeDomText(musicPlayerDom, "Title", item.m_itemName);

        foreach(const MusicSong &song, items[i].m_songs)
        {
            //Class B
            QDomElement trackDom = writeDomNode(musicPlayerDom, "Entry");

            writeDomText(trackDom, "Title", song.getMusicArtistBack());
            writeDomElement(trackDom, "Ref", MusicXmlAttribute("href", song.getMusicPath()));
            writeDomElement(trackDom, "Duration", MusicXmlAttribute("value", "00:" + song.getMusicPlayTime() + ".000"));
            writeDomText(trackDom, "Author", APP_NAME);
        }
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
}
