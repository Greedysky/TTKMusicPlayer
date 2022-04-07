#include "musicbackgroundconfigmanager.h"

MusicSkinConfigManager::MusicSkinConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicSkinConfigManager::readBuffer(MusicSkinItem &item)
{
    item.m_name = readXmlAttributeByTagNameValue("name");
    item.m_useCount = readXmlAttributeByTagNameValue("useCount").toInt();
}

void MusicSkinConfigManager::writeBuffer(const MusicSkinItem &item, const QString &path)
{
    if(!toFile(path))
    {
        return;
    }
    //
    createProcessingInstruction();
    //
    QDomElement musicPlayerDom = createRoot("TTKSkin");
    //Class A
    writeDomElement(musicPlayerDom, "creator", {"value", APP_NAME});
    writeDomElement(musicPlayerDom, "name", {"value", item.m_name});
    writeDomElement(musicPlayerDom, "useCount", {"value", item.m_useCount});

    QTextStream out(m_file);
    m_document->save(out, 4);
}
