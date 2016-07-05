#include "musicmydownloadrecordconfigmanager.h"

MusicMyDownloadRecordConfigManager::MusicMyDownloadRecordConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

QString MusicMyDownloadRecordConfigManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicMyDownloadRecordConfigManager::writeDownloadConfig(const MusicDownloadRecord &record)
{
    if( !writeConfig( DOWNLOADINFO_FULL ) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("TTKMusicPlayer");
    QDomElement download = writeDom(musicPlayer, "download");

    for(int i=0; i<record.m_names.count(); ++i)
    {
        writeDomElementMutilText(download, "value", QStringList()<<"name"<<"size",
                                 QList<QVariant>()<<record.m_names[i]<<record.m_sizes[i],
                                 record.m_paths[i]);
    }
    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicMyDownloadRecordConfigManager::readDownloadConfig(MusicDownloadRecord &record)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("value");
    QStringList names, paths, size;
    for(int i=0; i<nodelist.count(); ++i)
    {
        names << nodelist.at(i).toElement().attribute("name");
        paths << nodelist.at(i).toElement().text();
        size << nodelist.at(i).toElement().attribute("size");
    }
    record.m_names = names;
    record.m_paths = paths;
    record.m_sizes = size;
}
