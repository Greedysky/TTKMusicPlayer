#include "musicxspfconfigmanager.h"

MusicXSPFConfigManager::MusicXSPFConfigManager()
    : TTKAbstractXml(nullptr)
    , MusicPlaylistInterface()
{

}

bool MusicXSPFConfigManager::readBuffer(MusicSongItemList &items)
{
    TTKXmlNodeHelper helper(m_document->documentElement());
    helper.load();

    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file->fileName()).baseName();

    const QDomNodeList &trackNodes = m_document->elementsByTagName(helper.nodeName("trackList"));
    for(int i = 0; i < trackNodes.count(); ++i)
    {
        const QDomNode &node = trackNodes.at(i);
        const QDomNodeList &nodeList = node.childNodes();

        for(int i = 0; i < nodeList.count(); ++i)
        {
            const QDomNode &track = nodeList.at(i);
            const QDomNodeList &trackList = track.childNodes();

            QString duration, path;
            for(int i = 0; i < trackList.count(); ++i)
            {
                const QDomElement &element = trackList.at(i).toElement();
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
        QDomElement trackListDom = writeDomNode(rootDom, "trackList");
        for(const MusicSong &song : qAsConst(items[i].m_songs))
        {
            QDomElement trackDom = writeDomNode(trackListDom, "track");

            writeDomText(trackDom, "location", song.path());
            writeDomText(trackDom, "title", song.artistBack());
            writeDomText(trackDom, "creator", song.artistFront());
            writeDomText(trackDom, "duration", song.playTime());
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
