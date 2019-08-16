#include "musiccategoryconfigmanager.h"

MusicCategoryConfigManager::MusicCategoryConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

bool MusicCategoryConfigManager::readConfig(Type type)
{
    QString v;
    switch(type)
    {
        case Playlist: v = ":/data/playlist"; break;
        case Toplist: v = ":/data/toplist"; break;
        case ArtistList: v = ":/data/artistlist"; break;
        case MovieList: v = ":/data/movielist"; break;
        default: break;
    }

    return MusicAbstractXml::readConfig(v);
}

void MusicCategoryConfigManager::readCategoryData(MusicResultsCategorys &records, const QString &key)
{
    const QDomNodeList &nodes = m_document->elementsByTagName(key);
    for(int i=0; i<nodes.count(); ++i)
    {
        const QDomNode &node = nodes.at(i);
        const QDomNodeList &tagNodes = node.childNodes();
        for(int j=0; j<tagNodes.count(); ++j)
        {
            MusicResultsCategory category;
            const QDomNode &tagNode = tagNodes.at(j);
            category.m_category = tagNode.toElement().attribute("value");
            const QDomNodeList &typeNodes = tagNode.childNodes();
            for(int k=0; k<typeNodes.count(); ++k)
            {
                MusicResultsCategoryItem item;
                const QDomNode &typeNode = typeNodes.at(k);
                item.m_name = typeNode.toElement().attribute("value");
                item.m_id = typeNode.toElement().attribute("key");
                category.m_items.append(item);
            }
            records.append(category);
        }
    }
}
