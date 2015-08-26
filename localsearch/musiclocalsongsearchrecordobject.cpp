#include "musiclocalsongsearchrecordobject.h"

MusicLocalSongSearchRecordObject::MusicLocalSongSearchRecordObject(QObject *parent)
    : MusicXmlAbstract(parent)
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
    m_ddom->appendChild(
        m_ddom->createProcessingInstruction("xml","version='1.0' encoding='UTF-8'") );
    QDomElement QMusicPlayer = m_ddom->createElement("QMusicPlayer");
    m_ddom->appendChild(QMusicPlayer);

    QDomElement download = m_ddom->createElement("searchRecord");
    QMusicPlayer.appendChild(download);
    for(int i=0; i<names.count(); ++i)
    {
        QDomElement value = m_ddom->createElement("value");
        value.setAttribute("name", names[i]);
        QDomText valuetext = m_ddom->createTextNode(times[i]);
        value.appendChild(valuetext);
        download.appendChild(value);
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
