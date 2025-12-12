#include "tfmxmetadatamodel.h"
#include "tfmxhelper.h"

TFMXMetaDataModel::TFMXMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    TFMXHelper helper(path);
    if(helper.initialize())
    {
        fillProperties(&helper);
    }
}

void TFMXMetaDataModel::fillProperties(TFMXHelper *helper)
{
    m_ep << MetaDataItem(tr("Format id"), helper->id());
    m_ep << MetaDataItem(tr("File name"), helper->fileName());
    m_ep << MetaDataItem(tr("Voice size"), helper->voices());

    QString value;
    for(int i = 0; i < helper->voices(); ++i)
    {
        value += QString::number(helper->voice(i));
        value += "\n";
    }

    if(!value.isEmpty())
    {
        m_desc << MetaDataItem(tr("Voices"), value);
    }
}

QList<MetaDataItem> TFMXMetaDataModel::extraProperties() const
{
    return m_ep;
}

QList<MetaDataItem> TFMXMetaDataModel::descriptions() const
{
    return m_desc;
}
