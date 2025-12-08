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
    m_ep << MetaDataItem(tr("TrackTitles"), helper->trackTitleCount());
    m_ep << MetaDataItem(tr("MetaMarkers"), helper->metaMarkerCount());
    m_ep << MetaDataItem(tr("Copyright"), helper->copyright());

    m_desc << MetaDataItem(tr("TrackTitles"), helper->trackTitles());
    m_desc << MetaDataItem(tr("MetaMarkers"), helper->metaMarkers());
}

QList<MetaDataItem> AdlMidiMetaDataModel::extraProperties() const
{
    return m_ep;
}

QList<MetaDataItem> AdlMidiMetaDataModel::descriptions() const
{
    return m_desc;
}
