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
    m_ep << MetaDataItem(tr("Instruments"), helper->instruments());

    QString value;
    for(int i = 0; i < helper->instruments(); ++i)
    {
        value += helper->instrument(i);
        value += "\n";
    }

    if(!value.isEmpty())
    {
        m_desc << MetaDataItem(tr("Instruments"), value);
    }
}

QList<MetaDataItem> BuzzicMetaDataModel::extraProperties() const
{
    return m_ep;
}

QList<MetaDataItem> BuzzicMetaDataModel::descriptions() const
{
    return m_desc;
}
