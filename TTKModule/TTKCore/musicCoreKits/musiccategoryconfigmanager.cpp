#include "musiccategoryconfigmanager.h"

MusicPlaylistCategoryConfigManager::MusicPlaylistCategoryConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

QString MusicPlaylistCategoryConfigManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicPlaylistCategoryConfigManager::readCategoryConfig(MusicPlaylistCategorys &records, const QString &key)
{
    QDomNodeList nodes = m_ddom->elementsByTagName(key);
    for(int i=0; i<nodes.count(); ++i)
    {
        QDomNode node = nodes.at(i);
        QDomNodeList tagNodes = node.childNodes();
        for(int j=0; j<tagNodes.count(); ++j)
        {
            MusicPlaylistCategory category;
            QDomNode tagNode = tagNodes.at(j);
            category.m_category = tagNode.toElement().attribute("value");
            QDomNodeList typeNodes = tagNode.childNodes();
            for(int k=0; k<typeNodes.count(); ++k)
            {
                MusicPlaylistCategoryItem item;
                QDomNode typeNode = typeNodes.at(k);
                item.m_name = typeNode.toElement().attribute("value");
                item.m_id = typeNode.toElement().attribute("key");
                category.m_items.append(item);
            }
            records.append(category);
        }
    }
}



MusicToplistCategoryConfigManager::MusicToplistCategoryConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

QString MusicToplistCategoryConfigManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicToplistCategoryConfigManager::readCategoryConfig(MusicPlaylistCategorys &records, const QString &key)
{
    QDomNodeList nodes = m_ddom->elementsByTagName(key);
    for(int i=0; i<nodes.count(); ++i)
    {
        QDomNode node = nodes.at(i);
        QDomNodeList tagNodes = node.childNodes();
        for(int j=0; j<tagNodes.count(); ++j)
        {
            MusicPlaylistCategory category;
            QDomNode tagNode = tagNodes.at(j);
            category.m_category = tagNode.toElement().attribute("value");
            QDomNodeList typeNodes = tagNode.childNodes();
            for(int k=0; k<typeNodes.count(); ++k)
            {
                MusicPlaylistCategoryItem item;
                QDomNode typeNode = typeNodes.at(k);
                item.m_name = typeNode.toElement().attribute("value");
                item.m_id = typeNode.toElement().attribute("key");
                category.m_items.append(item);
            }
            records.append(category);
        }
    }
}
