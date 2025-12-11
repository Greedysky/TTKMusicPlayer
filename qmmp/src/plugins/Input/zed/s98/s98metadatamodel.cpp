#include "s98metadatamodel.h"
#include "s98helper.h"

S98MetaDataModel::S98MetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    S98Helper helper(path);
    if(helper.initialize() && helper.hasMetaData())
    {
        fillProperties(&helper);
    }
}

void S98MetaDataModel::fillProperties(S98Helper *helper)
{
    m_ep << MetaDataItem("Game", helper->metaData("game"));
    m_ep << MetaDataItem("System", helper->metaData("system"));
    m_ep << MetaDataItem("Copyright", helper->metaData("copyright"));
}

QList<MetaDataItem> S98MetaDataModel::extraProperties() const
{
    return m_ep;
}
