#include "musiclocalsongsearchrecordobject.h"

MusicLocalSongSearchRecordObject::MusicLocalSongSearchRecordObject(QObject *parent)
    : MusicAbstractXml(parent)
{

}


void MusicLocalSongSearchRecordObject::writeSearchConfig(const QStringList &names,
                                                         const QStringList &times)
{
    if( !writeConfig( MUSICSEARCH ) )
    {
        return;
    }

    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("QMusicPlayer");
    QDomElement download = writeDom(musicPlayer, "searchRecord");

    for(int i=0; i<names.count(); ++i)
    {
        writeDomElementText(download, "value", "name", names[i], times[i]);
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out,4);
}

void MusicLocalSongSearchRecordObject::readSearchConfig(QStringList &names, QStringList &times)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("value");
    for(int i=0; i<nodelist.count(); ++i)
    {
        names<<nodelist.at(i).toElement().attribute("name");
        times<<nodelist.at(i).toElement().text();
    }
}
