#include "musiccategoryconfigmanager.h"

MusicCategoryConfigManager::MusicCategoryConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

QString MusicCategoryConfigManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicCategoryConfigManager::readCategoryConfig(PlaylistCategorys &records, const QString &key)
{
    QDomNodeList nodes = m_ddom->elementsByTagName(key);
    for(int i=0; i<nodes.count(); ++i)
    {
        QDomNode node = nodes.at(i);
        QDomNodeList tagNodes = node.childNodes();
        for(int j=0; j<tagNodes.count(); ++j)
        {
            PlaylistCategory category;
            QDomNode tagNode = tagNodes.at(j);
            category.m_category = tagNode.toElement().attribute("value");
            QDomNodeList typeNodes = tagNode.childNodes();
            for(int k=0; k<typeNodes.count(); ++k)
            {
                PlaylistCategoryItem item;
                QDomNode typeNode = typeNodes.at(k);
                item.m_name = typeNode.toElement().attribute("value");
                item.m_id = typeNode.toElement().attribute("key");
                category.m_items.append(item);
            }
            records.append(category);
        }
    }
}
