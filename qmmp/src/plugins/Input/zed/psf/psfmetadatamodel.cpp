#include "psfmetadatamodel.h"
#include "psfhelper.h"

PSFMetaDataModel::PSFMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    PSFHelper helper(path);
    if(helper.initialize() && helper.hasMetaData())
    {
        fillProperties(&helper);
    }
}

void PSFMetaDataModel::fillProperties(PSFHelper *helper)
{
    m_ep << MetaDataItem("Game", helper->metaData("game"));
    m_ep << MetaDataItem("Fade", helper->metaData("fade"));
    m_ep << MetaDataItem("Ripper", helper->metaData("ripper"));
    m_ep << MetaDataItem("Copyright", helper->metaData("copyright"));
}

QList<MetaDataItem> PSFMetaDataModel::extraProperties() const
{
    return m_ep;
}
