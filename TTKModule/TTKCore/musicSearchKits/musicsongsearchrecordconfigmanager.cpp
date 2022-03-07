#include "musicsongsearchrecordconfigmanager.h"

MusicSongSearchRecordConfigManager::MusicSongSearchRecordConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicSongSearchRecordConfigManager::readSearchData(MusicSearchRecordList &records)
{
    const QDomNodeList &nodeList = m_document->elementsByTagName("value");
    for(int i=0; i<nodeList.count(); ++i)
    {
        MusicSearchRecord record;
        record.m_name = nodeList.at(i).toElement().attribute("name");
        record.m_timestamp = nodeList.at(i).toElement().text();
        records << record;
    }
}

void MusicSongSearchRecordConfigManager::writeSearchData(const MusicSearchRecordList &records)
{
    if(!writeConfig(SEARCH_PATH_FULL))
    {
        return;
    }

    //
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot(APP_NAME);
    QDomElement download = writeDomNode(musicPlayer, "record");

    for(const MusicSearchRecord &record : qAsConst(records))
    {
        writeDomElementText(download, "value", MusicXmlAttribute("name", record.m_name), record.m_timestamp);
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
}
