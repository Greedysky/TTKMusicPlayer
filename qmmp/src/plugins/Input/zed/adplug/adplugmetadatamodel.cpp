#include "adplugmetadatamodel.h"
#include "adplughelper.h"

AdplugMetaDataModel::AdplugMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    AdplugHelper helper(path);
    if(helper.initialize())
    {
        fillProperties(&helper);
    }
}

void AdplugMetaDataModel::fillProperties(AdplugHelper *helper)
{
    m_ep << MetaDataItem(tr("Format"), helper->format());
    m_ep << MetaDataItem(tr("Description"), helper->description());
    m_ep << MetaDataItem(tr("Patterns"), helper->patternCount());
    m_ep << MetaDataItem(tr("Instruments"), helper->instrumentCount());

    m_desc << MetaDataItem(tr("Instruments"), helper->instruments());
}

QList<MetaDataItem> AdplugMetaDataModel::extraProperties() const
{
    return m_ep;
}

QList<MetaDataItem> AdplugMetaDataModel::descriptions() const
{
    return m_desc;
}
