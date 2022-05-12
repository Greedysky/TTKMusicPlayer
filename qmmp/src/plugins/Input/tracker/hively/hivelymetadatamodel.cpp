#include "hivelymetadatamodel.h"
#include "hivelyhelper.h"

HivelyMetaDataModel::HivelyMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    HivelyHelper helper(path);
    if(helper.initialize())
    {
        fillProperties(&helper);
    }
}

void HivelyMetaDataModel::fillProperties(HivelyHelper *helper)
{
    m_ep << MetaDataItem(tr("Format"), helper->format());
    m_ep << MetaDataItem(tr("SubSongs"), helper->subSongCount());
    m_ep << MetaDataItem(tr("Instruments"), helper->instrumentCount());

    m_desc << MetaDataItem(tr("SubSongs"), helper->subSongs());
    m_desc << MetaDataItem(tr("Instruments"), helper->instruments());
}

QList<MetaDataItem> HivelyMetaDataModel::extraProperties() const
{
    return m_ep;
}

QList<MetaDataItem> HivelyMetaDataModel::descriptions() const
{
    return m_desc;
}
