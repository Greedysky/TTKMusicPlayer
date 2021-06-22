#include "adplugmetadatamodel.h"
#include "adplughelper.h"

AdplugMetaDataModel::AdplugMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    AdplugHelper helper(qUtf8Printable(path));
    if(helper.initialize())
    {
        fill_in_extra_properties(&helper);
        fill_in_descriptions(&helper);
    }
}

void AdplugMetaDataModel::fill_in_extra_properties(AdplugHelper *helper)
{
    if(helper->instrument_count() != 0)
    {
        QString text;
        for(const QString &s : helper->instruments())
        {
            text += s + "\n";
        }
        m_desc << MetaDataItem(tr("Instruments"), text);
    }
}

void AdplugMetaDataModel::fill_in_descriptions(AdplugHelper *helper)
{
    m_ap << MetaDataItem(tr("Title"), helper->title());
    m_ap << MetaDataItem(tr("Format"), helper->format());
    m_ap << MetaDataItem(tr("Author"), helper->author());
    m_ap << MetaDataItem(tr("Description"), helper->author());

    if(helper->pattern_count() != 0)
    {
        m_ap << MetaDataItem(tr("Patterns"), QString::number(helper->pattern_count()));
    }
    if(helper->instrument_count() != 0)
    {
        m_ap << MetaDataItem(tr("Instruments"), QString::number(helper->instrument_count()));
    }
}

QList<MetaDataItem> AdplugMetaDataModel::extraProperties() const
{
    return m_ap;
}

QList<MetaDataItem> AdplugMetaDataModel::descriptions() const
{
    return m_desc;
}
