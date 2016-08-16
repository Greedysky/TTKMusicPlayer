#include "musicmydownloadrecordconfigmanager.h"

MusicMyDownloadRecordConfigManager::MusicMyDownloadRecordConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

QString MusicMyDownloadRecordConfigManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicMyDownloadRecordConfigManager::writeDownloadConfig(const MusicDownloadRecords &records)
{
    if( !writeConfig( DOWNLOADINFO_FULL ) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("TTKMusicPlayer");
    QDomElement download = writeDom(musicPlayer, "download");

    foreach(const MusicDownloadRecord &record, records)
    {
        writeDomElementMutilText(download, "value", QStringList() << "name" << "size",
                                 QVariantList() << record.m_name << record.m_size, record.m_path);
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicMyDownloadRecordConfigManager::readDownloadConfig(MusicDownloadRecords &records)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("value");
    for(int i=0; i<nodelist.count(); ++i)
    {
        MusicDownloadRecord record;
        record.m_name = nodelist.at(i).toElement().attribute("name");
        record.m_path = nodelist.at(i).toElement().text();
        record.m_size = nodelist.at(i).toElement().attribute("size");
        records << record;
    }
}
