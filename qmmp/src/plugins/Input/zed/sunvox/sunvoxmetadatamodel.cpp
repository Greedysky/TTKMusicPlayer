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
    m_ep << MetaDataItem(tr("BPM"), helper->beatsPerMinute());
    m_ep << MetaDataItem(tr("TPL"), helper->ticksPerLine());
    m_ep << MetaDataItem(tr("Module size"), helper->modules());
    m_ep << MetaDataItem(tr("Pattern size"), helper->patterns());

    m_desc << MetaDataItem(tr("Modules"), helper->module());
    m_desc << MetaDataItem(tr("Patterns"), helper->pattern());
}

QList<MetaDataItem> SunVoxMetaDataModel::extraProperties() const
{
    return m_ep;
}

QList<MetaDataItem> SunVoxMetaDataModel::descriptions() const
{
    return m_desc;
}
