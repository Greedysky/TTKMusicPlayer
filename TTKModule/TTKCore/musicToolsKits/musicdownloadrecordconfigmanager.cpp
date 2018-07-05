#include "musicdownloadrecordconfigmanager.h"

MusicDownloadRecordConfigManager::MusicDownloadRecordConfigManager(MusicObject::RecordType type, QObject *parent)
    : MusicAbstractXml(parent)
{
    m_type = type;
}

void MusicDownloadRecordConfigManager::writeDownloadConfig(const MusicSongs &records)
{
    if(!writeConfig(mappingFilePathFromEnum()))
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot(APPNAME);
    QDomElement download = writeDom(musicPlayer, "download");

    foreach(const MusicSong &record, records)
    {
        writeDomElementMutilText(download, "value", MusicXmlAttributes() << MusicXmlAttribute("name", record.getMusicName())
                                                 << MusicXmlAttribute("size", record.getMusicSizeStr())
                                                 << MusicXmlAttribute("time", record.getMusicAddTimeStr()),
                                                    record.getMusicPath());
    }

    //Write to file
    QTextStream out(m_file);
    m_document->save(out, 4);
}

void MusicDownloadRecordConfigManager::readDownloadConfig(MusicSongs &records)
{
    QDomNodeList nodelist = m_document->elementsByTagName("value");
    for(int i=0; i<nodelist.count(); ++i)
    {
        MusicSong record;
        record.setMusicName(nodelist.at(i).toElement().attribute("name"));
        record.setMusicSizeStr(nodelist.at(i).toElement().attribute("size"));
        record.setMusicAddTimeStr(nodelist.at(i).toElement().attribute("time"));
        record.setMusicPath(nodelist.at(i).toElement().text());
        records << record;
    }
}

QString MusicDownloadRecordConfigManager::mappingFilePathFromEnum() const
{
    switch(m_type)
    {
        case MusicObject::RecordNormalDownload : return NORMALDOWNPATH_FULL;
        case MusicObject::RecordCloudDownload  : return CLOUDDOWNPATH_FULL;
        case MusicObject::RecordCloudUpload    : return CLOUDUPPATH_FULL;
        default: return QString();
    }
}
