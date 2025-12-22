#include "adlmidimetadatamodel.h"
#include "adlmidihelper.h"

AdlMidiMetaDataModel::AdlMidiMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    AdlMidiHelper helper(path);
    if(helper.initialize())
    {
        fillProperties(&helper);
    }
}

void AdlMidiMetaDataModel::fillProperties(AdlMidiHelper *helper)
{
    m_ep << MetaDataItem(tr("Track title size"), helper->trackTitles());
    m_ep << MetaDataItem(tr("Meta marker size"), helper->metaMarkers());
    m_ep << MetaDataItem(tr("Copyright"), helper->copyright());

    QString value;
    for(int i = 0; i < helper->trackTitles(); ++i)
    {
        const QString &v = helper->trackTitle(i);
        if(!v.isEmpty())
        {
            value += v + "\n";
        }
    }

    if(!value.isEmpty())
    {
        value.chop(1); // last \n
        m_desc << MetaDataItem(tr("Titles"), value);
        value.clear();
    }

    for(int i = 0; i < helper->metaMarkers(); ++i)
    {
        const QString &v = helper->metaMarker(i);
        if(!v.isEmpty())
        {
            value += v + "\n";
        }
    }

    if(!value.isEmpty())
    {
        value.chop(1); // last \n
        m_desc << MetaDataItem(tr("Markers"), value);
    }
}

QList<MetaDataItem> AdlMidiMetaDataModel::extraProperties() const
{
    return m_ep;
}

QList<MetaDataItem> AdlMidiMetaDataModel::descriptions() const
{
    return m_desc;
}
