#include "musicbackgroundconfigmanager.h"

MusicSkinConfigManager::MusicSkinConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

QString MusicSkinConfigManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicSkinConfigManager::writeSkinXMLConfig(const MusicSkinConfigItem &item, const QString &path)
{
    //Open wirte file
    if( !writeConfig( path ) )
    {
        return;
    }
    ///////////////////////////////////////////////////////
    createProcessingInstruction();
    ///////////////////////////////////////////////////////
    QDomElement musicPlayerDom = createRoot("TTKSkin");
    //Class A
    writeDomElement(musicPlayerDom, "creator", MusicXmlAttribute("value", APPNAME));
    writeDomElement(musicPlayerDom, "name", MusicXmlAttribute("value", item.m_name));
    writeDomElement(musicPlayerDom, "useCount", MusicXmlAttribute("value", item.m_useCount));

    //Write to file
    QTextStream out(m_file);
    m_ddom->save(out, 4);
}

void MusicSkinConfigManager::readSkinXMLConfig(MusicSkinConfigItem &item)
{
    item.m_name = readXmlAttributeByTagNameValue("name");
    item.m_useCount = readXmlAttributeByTagNameValue("useCount").toInt();
}
