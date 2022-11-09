#include "adplugmetadatamodel.h"
#include "adplughelper.h"

AdPlugMetaDataModel::AdPlugMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    AdPlugHelper helper(path);
    if(helper.initialize())
    {
        fillProperties(&helper);
    }
}

void AdPlugMetaDataModel::fillProperties(AdPlugHelper *helper)
{
    m_ep << MetaDataItem(tr("Format"), helper->format());
    m_ep << MetaDataItem(tr("Description"), helper->description());
    m_ep << MetaDataItem(tr("Patterns"), helper->patternCount());
    m_ep << MetaDataItem(tr("Instruments"), helper->instrumentCount());

    m_desc << MetaDataItem(tr("Instruments"), helper->instruments());
}

QList<MetaDataItem> AdPlugMetaDataModel::extraProperties() const
{
    return m_ep;
}

QList<MetaDataItem> AdPlugMetaDataModel::descriptions() const
{
    return m_desc;
}
