#include "musiclocalsongsearchrecordconfigmanager.h"

MusicLocalSongSearchRecordConfigManager::MusicLocalSongSearchRecordConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicLocalSongSearchRecordConfigManager::readSearchData(MusicSearchRecords &records)
{
    const QDomNodeList &nodeList = m_document->elementsByTagName("value");
    for(int i=0; i<nodeList.count(); ++i)
    {
        MusicSearchRecord record;
        record.m_name = nodeList.at(i).toElement().attribute("name");
        record.m_time = nodeList.at(i).toElement().text();
        records << record;
    }
}

void MusicLocalSongSearchRecordConfigManager::writeSearchData(const MusicSearchRecords &records)
{
    if(!writeConfig(MUSICSEARCH_FULL))
    {
        return;
    }

    //
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot(APP_NAME);
    QDomElement download = writeDomNode(musicPlayer, "searchRecord");

    foreach(const MusicSearchRecord &record, records)
    {
        writeDomElementText(download, "value", MusicXmlAttribute("name", record.m_name), record.m_time);
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
}
