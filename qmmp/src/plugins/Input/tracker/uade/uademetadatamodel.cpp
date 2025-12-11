#include "uademetadatamodel.h"
#include "uadehelper.h"

UADEMetaDataModel::UADEMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    UADEHelper helper(path);
    if(helper.initialize() && helper.hasMetaData())
    {
        fillProperties(&helper);
    }
}

void UADEMetaDataModel::fillProperties(UADEHelper *helper)
{
    m_ep << MetaDataItem(tr("Module MD5"), helper->metaData("modulemd5"));
    m_ep << MetaDataItem(tr("Module name"), helper->metaData("modulename"));
    m_ep << MetaDataItem(tr("Module path"), helper->metaData("modulepath"));
    m_ep << MetaDataItem(tr("Format"), helper->metaData("format"));
    m_ep << MetaDataItem(tr("Format name"), helper->metaData("formatname"));
    m_ep << MetaDataItem(tr("Player name"), helper->metaData("playername"));
}

QList<MetaDataItem> UADEMetaDataModel::extraProperties() const
{
    return m_ep;
}
