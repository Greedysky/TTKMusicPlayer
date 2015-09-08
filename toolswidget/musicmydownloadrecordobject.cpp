#include "musicmydownloadrecordobject.h"

MusicMyDownloadRecordObject::MusicMyDownloadRecordObject(QObject *parent) :
    MusicXmlAbstract(parent)
{

}

void MusicMyDownloadRecordObject::writeDownloadConfig(const QStringList &names,
                                                      const QStringList &paths)
{
    if( !writeConfig( DOWNLOADINFO ) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("QMusicPlayer");
    QDomElement download = writeDom(musicPlayer, "download");

    for(int i=0; i<names.count(); ++i)
    {
        writeDomEleText(download, "value", "value", names[i], paths[i]);
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
