#include "adplugmetadatamodel.h"
#include "adplughelper.h"

AdplugMetaDataModel::AdplugMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    AdplugHelper helper(path.toUtf8().constData());
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
        for(const std::string &s : helper->instruments())
        {
            text += QString::fromStdString(s) + "\n";
        }
        m_desc << MetaDataItem(tr("Instruments"), text);
    }
}

void AdplugMetaDataModel::fill_in_descriptions(AdplugHelper *helper)
{
    m_ap << MetaDataItem(tr("Title"), QString::fromStdString(helper->title()));
    m_ap << MetaDataItem(tr("Format"), QString::fromStdString(helper->format()));
    m_ap << MetaDataItem(tr("Author"), QString::fromStdString(helper->author()));
    m_ap << MetaDataItem(tr("Description"), QString::fromStdString(helper->author()));

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
