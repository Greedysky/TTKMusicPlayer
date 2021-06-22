#include "adplugmetadatamodel.h"
#include "adplughelper.h"

AdplugMetaDataModel::AdplugMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    AdplugHelper helper(qPrintable(path));
    if(helper.initialize())
    {
        fillEextraProperties(&helper);
        fillDescriptions(&helper);
    }
}

void AdplugMetaDataModel::fillEextraProperties(AdplugHelper *helper)
{
    if(helper->instrumentCount() != 0)
    {
        QString text;
        for(const QString &s : helper->instruments())
        {
            text += s + "\n";
        }
        m_desc << MetaDataItem(tr("Instruments"), text);
    }
}

void AdplugMetaDataModel::fillDescriptions(AdplugHelper *helper)
{
    m_ap << MetaDataItem(tr("Title"), helper->title());
    m_ap << MetaDataItem(tr("Format"), helper->format());
    m_ap << MetaDataItem(tr("Author"), helper->author());
    m_ap << MetaDataItem(tr("Description"), helper->author());

    if(helper->patternCount() != 0)
    {
        m_ap << MetaDataItem(tr("Patterns"), QString::number(helper->patternCount()));
    }

    if(helper->instrumentCount() != 0)
    {
        m_ap << MetaDataItem(tr("Instruments"), QString::number(helper->instrumentCount()));
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
