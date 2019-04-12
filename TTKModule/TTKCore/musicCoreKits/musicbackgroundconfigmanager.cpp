#include "musicbackgroundconfigmanager.h"

MusicSkinConfigManager::MusicSkinConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicSkinConfigManager::readSkinData(MusicSkinConfigItem &item)
{
    item.m_name = readXmlAttributeByTagNameValue("name");
    item.m_useCount = readXmlAttributeByTagNameValue("useCount").toInt();
}

void MusicSkinConfigManager::writeSkinData(const MusicSkinConfigItem &item, const QString &path)
{
    if(!writeConfig( path ))
    {
        return;
    }
    //
    createProcessingInstruction();
    //
    QDomElement musicPlayerDom = createRoot("TTKSkin");
    //Class A
    writeDomElement(musicPlayerDom, "creator", MusicXmlAttribute("value", APP_NAME));
    writeDomElement(musicPlayerDom, "name", MusicXmlAttribute("value", item.m_name));
    writeDomElement(musicPlayerDom, "useCount", MusicXmlAttribute("value", item.m_useCount));

    QTextStream out(m_file);
    m_document->save(out, 4);
}
