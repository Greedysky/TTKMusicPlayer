#include "musicmydownloadrecordobject.h"

MusicMyDownloadRecordObject::MusicMyDownloadRecordObject(QObject *parent) :
    MusicXmlAbstract(parent)
{

}

void MusicMyDownloadRecordObject::writeDownloadConfig(const QStringList& names,
                                                      const QStringList& paths)
{
    if( !writeConfig( DOWNLOADINFO ) ) return;

    ///////////////////////////////////////////////////////
    m_ddom->appendChild(
        m_ddom->createProcessingInstruction("xml","version='1.0' encoding='UTF-8'") );
    QDomElement QMusicPlayer = m_ddom->createElement("QMusicPlayer");
    m_ddom->appendChild(QMusicPlayer);

    QDomElement download = m_ddom->createElement("download");
    QMusicPlayer.appendChild(download);
    for(int i=0; i<names.count(); ++i)
    {
        QDomElement value = m_ddom->createElement("value");
        value.setAttribute("value", names[i]);
        QDomText valuetext = m_ddom->createTextNode(paths[i]);
        value.appendChild(valuetext);
        download.appendChild(value);
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out,4);
}

void MusicMyDownloadRecordObject::readDownloadConfig(QStringList &name, QStringList &path)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("value");
    for(int i=0; i<nodelist.count(); ++i)
    {
        name<<nodelist.at(i).toElement().attribute("value");
        path<<nodelist.at(i).toElement().text();
    }
}
