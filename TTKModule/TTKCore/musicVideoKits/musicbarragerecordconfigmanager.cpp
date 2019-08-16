#include "musicbarragerecordconfigmanager.h"

MusicBarrageRecordConfigManager::MusicBarrageRecordConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicBarrageRecordConfigManager::readBarrageData(MusicBarrageRecords &records)
{
    const QDomNodeList &nodeList = m_document->elementsByTagName("value");
    for(int i=0; i<nodeList.count(); ++i)
    {
        MusicBarrageRecord record;
        record.m_color = nodeList.at(i).toElement().attribute("color");
        record.m_size = nodeList.at(i).toElement().attribute("size").toInt();
        record.m_value = nodeList.at(i).toElement().text();
        records << record;
    }
}

void MusicBarrageRecordConfigManager::writeBarrageData(const MusicBarrageRecords &records)
{
    if(!writeConfig(BARRAGEPATH_FULL))
    {
        return;
    }
    //
    createProcessingInstruction();
    //
    QDomElement musicPlayer = createRoot(APP_NAME);
    QDomElement download = writeDomNode(musicPlayer, "barrageRecord");

    foreach(const MusicBarrageRecord &record, records)
    {
        writeDomElementMutilText(download, "value", MusicXmlAttributes() << MusicXmlAttribute("color", record.m_color)
                                 << MusicXmlAttribute("size", record.m_size), record.m_value);
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
}
