#include "musicbackgroundconfigmanager.h"

MusicSkinConfigManager::MusicSkinConfigManager()
    : TTKAbstractXml()
{

}

bool MusicSkinConfigManager::readBuffer(MusicSkinItem &items)
{
    items.m_name = readAttributeByTagName("name");
    items.m_useCount = readAttributeByTagName("useCount").toInt();
    return true;
}

bool MusicSkinConfigManager::writeBuffer(const MusicSkinItem &items)
{
    createProcessingInstruction();
    QDomElement rootDom = createRoot("TTKSkin");

    writeDomElement(rootDom, "creator", {"value", TTK_APP_NAME});
    writeDomElement(rootDom, "name", {"value", items.m_name});
    writeDomElement(rootDom, "useCount", {"value", items.m_useCount});

    save();
    return true;
}
