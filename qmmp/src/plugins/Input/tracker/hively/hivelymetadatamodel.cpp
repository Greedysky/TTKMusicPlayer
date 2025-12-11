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
    m_ep << MetaDataItem(tr("SubSong size"), helper->subSongs());
    m_ep << MetaDataItem(tr("Instrument size"), helper->instruments());

    QString value;
    if(helper->subSongs() > 1)
    {
        for(int i = 0; i < helper->subSongs(); ++i)
        {
            value += helper->title();
            value += "\n";
        }

        if(!value.isEmpty())
        {
            m_desc << MetaDataItem(tr("Titles"), value);
            value.clear();
        }
    }

    // instruments starts from 1 in hively so skip 0
    for(int i = 1; i < helper->instruments(); ++i)
    {
        value += helper->instrument(i);
        value += "\n";
    }

    if(!value.isEmpty())
    {
        m_desc << MetaDataItem(tr("Instruments"), value);
    }
}

QList<MetaDataItem> HivelyMetaDataModel::extraProperties() const
{
    return m_ep;
}

QList<MetaDataItem> HivelyMetaDataModel::descriptions() const
{
    return m_desc;
}
