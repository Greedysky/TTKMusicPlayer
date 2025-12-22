#include "adplugmetadatamodel.h"
#include "adplughelper.h"

AdPlugMetaDataModel::AdPlugMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    AdPlugHelper helper(path);
    if(helper.initialize())
    {
        fillProperties(&helper);
    }
}

void AdPlugMetaDataModel::fillProperties(AdPlugHelper *helper)
{
    m_ep << MetaDataItem(tr("Format"), helper->format());
    m_ep << MetaDataItem(tr("Description"), helper->description());
    m_ep << MetaDataItem(tr("Pattern size"), helper->patterns());
    m_ep << MetaDataItem(tr("Instrument size"), helper->instruments());

    QString value;
    for(int i = 0; i < helper->instruments(); ++i)
    {
        const QString &v = helper->instrument(i);
        if(!v.isEmpty())
        {
            value += v + "\n";
        }
    }

    if(!value.isEmpty())
    {
        value.chop(1); // last \n
        m_desc << MetaDataItem(tr("Instruments"), value);
    }
}

QList<MetaDataItem> AdPlugMetaDataModel::extraProperties() const
{
    return m_ep;
}

QList<MetaDataItem> AdPlugMetaDataModel::descriptions() const
{
    return m_desc;
}
