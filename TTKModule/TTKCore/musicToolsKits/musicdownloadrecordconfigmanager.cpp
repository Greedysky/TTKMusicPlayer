#include "musicdownloadrecordconfigmanager.h"

MusicDownloadRecordConfigManager::MusicDownloadRecordConfigManager(TTK::Record type, QObject *parent)
    : TTKXmlDocument(parent),
      m_type(type)
{

}

void MusicDownloadRecordConfigManager::readBuffer(MusicSongList &items)
{
    const QDomNodeList &nodes = m_document->elementsByTagName("value");
    for(int i = 0; i < nodes.count(); ++i)
    {
        const QDomElement &element = nodes.item(i).toElement();

        MusicSong record;
        record.setName(element.attribute("name"));
        record.setSizeStr(element.attribute("size"));
        record.setAddTimeStr(element.attribute("time"));
        record.setPath(element.text());
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
    QDomElement rootDom = createRoot(TTK_APP_NAME);
    QDomElement recordDom = writeDomNode(rootDom, "record");

    for(const MusicSong &record : qAsConst(items))
    {
        writeDomMutilElementText(recordDom, "value", {{"name", record.name()},
                                                      {"size", record.sizeStr()},
                                                      {"time", record.addTimeStr()}}, record.path());
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
}

QString MusicDownloadRecordConfigManager::mappingFilePathFromEnum() const
{
    switch(m_type)
    {
        case TTK::Record::NormalDownload: return NORMAL_DOWN_PATH_FULL;
        case TTK::Record::CloudDownload: return CLOUD_DOWN_PATH_FULL;
        case TTK::Record::CloudUpload: return CLOUD_UP_PATH_FULL;
        default: return {};
    }
}
