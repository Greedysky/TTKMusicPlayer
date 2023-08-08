#include "musicsongsearchrecordconfigmanager.h"

MusicSongSearchRecordConfigManager::MusicSongSearchRecordConfigManager(QObject *parent)
    : TTKXmlDocument(parent)
{

}

void MusicSongSearchRecordConfigManager::readBuffer(MusicSearchRecordList &items)
{
    const QDomNodeList &nodes = m_document->elementsByTagName("value");
    for(int i = 0; i < nodes.count(); ++i)
    {
        const QDomElement &element = nodes.item(i).toElement();

        MusicSearchRecord record;
        record.m_name = element.attribute("name");
        record.m_timestamp = element.text();
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
    QDomElement rootDom = createRoot(TTK_APP_NAME);
    QDomElement recordDom = writeDomNode(rootDom, "record");

    for(const MusicSearchRecord &record : qAsConst(items))
    {
        writeDomElementText(recordDom, "value", {"name", record.m_name}, record.m_timestamp);
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
}
