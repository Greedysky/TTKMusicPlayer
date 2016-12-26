#include "musicclouddownloadrecordconfigmanager.h"

MusicCloudDownloadRecordConfigManager::MusicCloudDownloadRecordConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

QString MusicCloudDownloadRecordConfigManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicCloudDownloadRecordConfigManager::writeDownloadConfig(const MusicCloudDownloadRecords &records)
{
    if( !writeConfig( CLOUDDOWNPATH_FULL ) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("TTKMusicPlayer");
    QDomElement download = writeDom(musicPlayer, "cloud");

    foreach(const MusicCloudDownloadRecord &record, records)
    {
        writeDomElementMutilText(download, "value", QStringList() << "name" << "size",
                                 QVariantList() << record.m_name << record.m_size, record.m_path);
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicCloudDownloadRecordConfigManager::readDownloadConfig(MusicCloudDownloadRecords &records)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("value");
    for(int i=0; i<nodelist.count(); ++i)
    {
        MusicCloudDownloadRecord record;
        record.m_name = nodelist.at(i).toElement().attribute("name");
        record.m_path = nodelist.at(i).toElement().text();
        record.m_size = nodelist.at(i).toElement().attribute("size");
        records << record;
    }
}
