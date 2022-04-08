#include "musicsongsearchrecordconfigmanager.h"

MusicSongSearchRecordConfigManager::MusicSongSearchRecordConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicSongSearchRecordConfigManager::readBuffer(MusicSearchRecordList &items)
{
    const QDomNodeList &nodeList = m_document->elementsByTagName("value");
    for(int i = 0; i < nodeList.count(); ++i)
    {
        MusicSearchRecord record;
        record.m_name = nodeList.at(i).toElement().attribute("name");
        record.m_timestamp = nodeList.at(i).toElement().text();
        items << record;
    }
}

void MusicSongSearchRecordConfigManager::writeBuffer(const MusicSearchRecordList &items)
{
    if(!toFile(SEARCH_PATH_FULL))
    {
        return;
    }

    createProcessingInstruction();
    QDomElement rootDom = createRoot(APP_NAME);
    QDomElement download = writeDomNode(rootDom, "record");

    for(const MusicSearchRecord &record : qAsConst(items))
    {
        writeDomElementText(download, "value", {"name", record.m_name}, record.m_timestamp);
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
}
