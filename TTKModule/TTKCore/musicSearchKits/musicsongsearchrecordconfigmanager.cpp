#include "musicsongsearchrecordconfigmanager.h"

MusicSongSearchRecordConfigManager::MusicSongSearchRecordConfigManager()
    : TTKAbstractXml()
{

}

bool MusicSongSearchRecordConfigManager::readBuffer(MusicSearchRecordList &items)
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

    return true;
}

bool MusicSongSearchRecordConfigManager::writeBuffer(const MusicSearchRecordList &items)
{
    createProcessingInstruction();
    QDomElement rootDom = createRoot(TTK_APP_NAME);
    QDomElement recordDom = writeDomElement(rootDom, "record");

    for(const MusicSearchRecord &record : qAsConst(items))
    {
        writeDomElement(recordDom, "value", {"name", record.m_name}, record.m_timestamp);
    }

    save();
    return true;
}
