#include "buzzicmetadatamodel.h"
#include "buzzichelper.h"

BuzzicMetaDataModel::BuzzicMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    BuzzicHelper helper(path);
    if(helper.initialize())
    {
        fillProperties(&helper);
    }
}

void BuzzicMetaDataModel::fillProperties(BuzzicHelper *helper)
{
    m_ep << MetaDataItem(tr("Instruments"), QString::number(helper->instrumentCount()));

    m_desc << MetaDataItem(tr("Instruments"), helper->instruments());
}

QList<MetaDataItem> BuzzicMetaDataModel::extraProperties() const
{
    return m_ep;
}

QList<MetaDataItem> BuzzicMetaDataModel::descriptions() const
{
    return m_desc;
}
