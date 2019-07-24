#include "musicdownloadrecordconfigmanager.h"

MusicDownloadRecordConfigManager::MusicDownloadRecordConfigManager(MusicObject::RecordType type, QObject *parent)
    : MusicAbstractXml(parent)
{
    m_type = type;
}

void MusicDownloadRecordConfigManager::readDownloadData(MusicSongs &records)
{
    const QDomNodeList &nodeList = m_document->elementsByTagName("value");
    for(int i=0; i<nodeList.count(); ++i)
    {
        MusicSong record;
        record.setMusicName(nodeList.at(i).toElement().attribute("name"));
        record.setMusicSizeStr(nodeList.at(i).toElement().attribute("size"));
        record.setMusicAddTimeStr(nodeList.at(i).toElement().attribute("time"));
        record.setMusicPath(nodeList.at(i).toElement().text());
        records << record;
    }
}

void MusicDownloadRecordConfigManager::writeDownloadData(const MusicSongs &records)
{
    if(!writeConfig(mappingFilePathFromEnum()))
    {
        return;
    }
    //
    createProcessingInstruction();
    QDomElement musicPlayer = createRoot(APP_NAME);
    QDomElement download = writeDomNode(musicPlayer, "download");

    foreach(const MusicSong &record, records)
    {
        writeDomElementMutilText(download, "value", MusicXmlAttributes() << MusicXmlAttribute("name", record.getMusicName())
                                                 << MusicXmlAttribute("size", record.getMusicSizeStr())
                                                 << MusicXmlAttribute("time", record.getMusicAddTimeStr()), record.getMusicPath());
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
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
