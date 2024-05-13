#include "musicfmconfigmanager.h"

MusicFMConfigManager::MusicFMConfigManager()
    : TTKAbstractXml()
{

}

bool MusicFMConfigManager::readBuffer(MusicFMCategoryList &items)
{
    const QDomNodeList &nodes = m_document->elementsByTagName("category");
    for(int i = 0; i < nodes.count(); ++i)
    {
        MusicFMCategory category;
        const QDomNode &node = nodes.item(i);
        category.m_category = node.toElement().attribute("value");

        const QDomNodeList &chnNodes = node.childNodes();
        for(int j = 0; j < chnNodes.count(); ++j)
        {
            const QDomElement &element = chnNodes.item(j).toElement();

            MusicFMChannel item;
            item.m_name = element.attribute("name");
            item.m_url = element.attribute("url");
            category.m_items << item;
        }
        items.append(category);
    }

    return true;
}

bool MusicFMConfigManager::writeBuffer(const MusicFMCategoryList &items)
{
    if(items.isEmpty())
    {
        return false;
    }

    createProcessingInstruction();
    QDomElement rootDom = createRoot(TTK_APP_NAME);

    for(const MusicFMCategory &item : qAsConst(items))
    {
        QDomElement categoryDom = writeDomElement(rootDom, "category", {"value", item.m_category});

        for(const MusicFMChannel &channel : qAsConst(item.m_items))
        {
            writeDomMultiElement(categoryDom, "channel", {{"name", channel.m_name}, {"url", channel.m_url}});
        }
    }

    save();
    return true;
}
