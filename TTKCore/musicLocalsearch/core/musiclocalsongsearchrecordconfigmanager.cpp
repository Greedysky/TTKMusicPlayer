#include "musiclocalsongsearchrecordconfigmanager.h"

MusicLocalSongSearchRecordConfigManager::MusicLocalSongSearchRecordConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}


void MusicLocalSongSearchRecordConfigManager::writeSearchConfig(const MusicSearchRecord &record)
{
    if( !writeConfig( MUSICSEARCH_AL ) )
    {
        return;
    }

    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot("TTKMusicPlayer");
    QDomElement download = writeDom(musicPlayer, "searchRecord");

    for(int i=0; i<record.m_names.count(); ++i)
    {
        writeDomElementText(download, "value", "name",
                            record.m_names[i], record.m_times[i]);
    }

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicLocalSongSearchRecordConfigManager::readSearchConfig(MusicSearchRecord &record)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("value");
    QStringList names, times;
    for(int i=0; i<nodelist.count(); ++i)
    {
        names << nodelist.at(i).toElement().attribute("name");
        times << nodelist.at(i).toElement().text();
    }
    record.m_names = names;
    record.m_times = times;
}
