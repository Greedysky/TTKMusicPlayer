#include "musicdownloadrecordconfigmanager.h"

namespace TTK
{
QString toString(Record type)
{
    switch(type)
    {
        case TTK::Record::NormalDownload: return NORMAL_DOWN_PATH_FULL;
        case TTK::Record::CloudDownload: return CLOUD_DOWN_PATH_FULL;
        case TTK::Record::CloudUpload: return CLOUD_UP_PATH_FULL;
        default: return {};
    }
}
}


MusicDownloadRecordConfigManager::MusicDownloadRecordConfigManager(QObject *parent)
    : TTKXmlDocument(parent)
{

}

bool MusicDownloadRecordConfigManager::readBuffer(MusicSongList &items)
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

    return true;
}

bool MusicDownloadRecordConfigManager::writeBuffer(const MusicSongList &items)
{
    createProcessingInstruction();
    QDomElement rootDom = createRoot(TTK_APP_NAME);
    QDomElement recordDom = writeDomNode(rootDom, "record");

    for(const MusicSong &record : qAsConst(items))
    {
        writeDomMutilElementText(recordDom, "value", {{"name", record.name()},
                                                      {"size", record.sizeStr()},
                                                      {"time", record.addTimeStr()}}, record.path());
    }

    save();
    return true;
}
