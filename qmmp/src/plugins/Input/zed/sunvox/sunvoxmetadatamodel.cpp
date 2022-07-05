#include "sunvoxmetadatamodel.h"
#include "sunvoxhelper.h"

SunVoxMetaDataModel::SunVoxMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    SunVoxHelper helper(path);
    if(helper.initialize(true))
    {
        fillProperties(&helper);
    }
}

void SunVoxMetaDataModel::fillProperties(SunVoxHelper *helper)
{
    m_ep << MetaDataItem(tr("BPM"), QString::number(helper->beatsPerMinute()));
    m_ep << MetaDataItem(tr("TPL"), QString::number(helper->ticksPerLine()));
    m_ep << MetaDataItem(tr("Modules"), QString::number(helper->moduleCount()));
    m_ep << MetaDataItem(tr("Patterns"), QString::number(helper->patternCount()));

    m_desc << MetaDataItem(tr("Modules"), helper->modules());
    m_desc << MetaDataItem(tr("Patterns"), helper->patterns());
}

QList<MetaDataItem> SunVoxMetaDataModel::extraProperties() const
{
    return m_ep;
}

QList<MetaDataItem> SunVoxMetaDataModel::descriptions() const
{
    return m_desc;
}
