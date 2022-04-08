#include "musicdownloadrecordconfigmanager.h"

MusicDownloadRecordConfigManager::MusicDownloadRecordConfigManager(MusicObject::RecordType type, QObject *parent)
    : MusicAbstractXml(parent)
{
    m_type = type;
}

void MusicDownloadRecordConfigManager::readBuffer(MusicSongList &items)
{
    const QDomNodeList &nodeList = m_document->elementsByTagName("value");
    for(int i = 0; i < nodeList.count(); ++i)
    {
        MusicSong record;
        record.setMusicName(nodeList.at(i).toElement().attribute("name"));
        record.setMusicSizeStr(nodeList.at(i).toElement().attribute("size"));
        record.setMusicAddTimeStr(nodeList.at(i).toElement().attribute("time"));
        record.setMusicPath(nodeList.at(i).toElement().text());
        items << record;
    }
}

void MusicDownloadRecordConfigManager::writeBuffer(const MusicSongList &items)
{
    if(!toFile(mappingFilePathFromEnum()))
    {
        return;
    }

    createProcessingInstruction();
    QDomElement rootDom = createRoot(APP_NAME);
    QDomElement download = writeDomNode(rootDom, "record");

    for(const MusicSong &record : qAsConst(items))
    {
        writeDomMutilElementText(download, "value", {{"name", record.musicName()},
                                                     {"size", record.musicSizeStr()},
                                                     {"time", record.musicAddTimeStr()}}, record.musicPath());
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
}

QString MusicDownloadRecordConfigManager::mappingFilePathFromEnum() const
{
    switch(m_type)
    {
        case MusicObject::RecordNormalDownload: return NORMAL_DOWN_PATH_FULL;
        case MusicObject::RecordCloudDownload: return CLOUD_DOWN_PATH_FULL;
        case MusicObject::RecordCloudUpload: return CLOUD_UP_PATH_FULL;
        default: return QString();
    }
}
