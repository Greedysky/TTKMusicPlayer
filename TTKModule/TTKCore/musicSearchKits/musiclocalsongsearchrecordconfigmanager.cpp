#include "musiclocalsongsearchrecordconfigmanager.h"

MusicLocalSongSearchRecordConfigManager::MusicLocalSongSearchRecordConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicLocalSongSearchRecordConfigManager::writeSearchConfig(const MusicSearchRecords &records)
{
    if(!writeConfig(MUSICSEARCH_FULL))
    {
        return;
    }

    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot(APPNAME);
    QDomElement download = writeDom(musicPlayer, "searchRecord");

    foreach(const MusicSearchRecord &record, records)
    {
        writeDomElementText(download, "value", MusicXmlAttribute("name", record.m_name), record.m_time);
    }

    //Write to file
    QTextStream out(m_file);
    m_document->save(out, 4);
}

void MusicLocalSongSearchRecordConfigManager::readSearchConfig(MusicSearchRecords &records)
{
    QDomNodeList nodelist = m_document->elementsByTagName("value");
    for(int i=0; i<nodelist.count(); ++i)
    {
        MusicSearchRecord record;
        record.m_name = nodelist.at(i).toElement().attribute("name");
        record.m_time = nodelist.at(i).toElement().text();
        records << record;
    }
}
