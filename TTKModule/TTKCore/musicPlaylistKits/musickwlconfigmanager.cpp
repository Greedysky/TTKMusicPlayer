#include "musickwlconfigmanager.h"
#include "musicotherdefine.h"

MusicKWLConfigManager::MusicKWLConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
    , MusicPlaylistInterface()
{

}

bool MusicKWLConfigManager::readConfig(const QString &name)
{
    delete m_file;
    m_file = new QFile( name );
    if(!m_file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QByteArray data(m_file->readAll());
    data.insert(0, "<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n");
    m_file->close();

    if(!m_file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }
    m_file->write(data);
    m_file->close();

    return MusicAbstractXml::readConfig(name);
}

void MusicKWLConfigManager::readPlaylistData(MusicSongItems &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file->fileName()).baseName();

    const QDomNodeList &nodes = m_document->elementsByTagName("so");
    for(int i=0; i<nodes.count(); ++i)
    {
        if(i == 0) //Skip root node
        {
            continue;
        }

        const QDomElement &element = nodes.at(i).toElement();
        const QDomNodeList &reslist = nodes.at(i).childNodes();
        for(int i=0; i<reslist.count(); i++)
        {
            const QDomElement &resElement = reslist.at(i).toElement();
            item.m_songs << MusicSong(resElement.attribute("p2pcachepath"),
                                      element.attribute("playedtimes").toInt(), MUSIC_TIME_INIT,
                                      element.attribute("artist") + " - " +
                                      element.attribute("name"));
            break;
        }
    }

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
}

void MusicKWLConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    Q_UNUSED(items);
    Q_UNUSED(path);
}
