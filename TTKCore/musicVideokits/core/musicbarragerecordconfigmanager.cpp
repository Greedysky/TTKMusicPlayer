#include "musicbarragerecordconfigmanager.h"

MusicBarrageRecordConfigManager::MusicBarrageRecordConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

QString MusicBarrageRecordConfigManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicBarrageRecordConfigManager::writeBarrageConfig(const MusicBarrageRecords &records)
{
    if( !writeConfig( BARRAGEPATH_FULL ) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("TTKMusicPlayer");
    QDomElement download = writeDom(musicPlayer, "barrageRecord");

    foreach(const MusicBarrageRecord &record, records)
    {
        writeDomElementMutilText(download, "value", QStringList() << "color" << "size",
                                 QVariantList() << record.m_color << record.m_size, record.m_value);
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicBarrageRecordConfigManager::readBarrageConfig(MusicBarrageRecords &records)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("value");
    for(int i=0; i<nodelist.count(); ++i)
    {
        MusicBarrageRecord record;
        record.m_color = nodelist.at(i).toElement().attribute("color");
        record.m_size = nodelist.at(i).toElement().attribute("size").toInt();
        record.m_value = nodelist.at(i).toElement().text();
        records << record;
    }
}
