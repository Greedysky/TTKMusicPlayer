#include "uademetadatamodel.h"
#include "uadehelper.h"

UADEMetaDataModel::UADEMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    Q_UNUSED(path);
}

QList<MetaDataItem> UADEMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;
    if(UADEHelper::instance()->hasTags())
    {
        ep << MetaDataItem("Module MD5", UADEHelper::instance()->tag("modulemd5"));
        ep << MetaDataItem("Module Name", UADEHelper::instance()->tag("modulename"));
        ep << MetaDataItem("Module Path", UADEHelper::instance()->tag("modulepath"));
        ep << MetaDataItem("Format", UADEHelper::instance()->tag("format"));
        ep << MetaDataItem("Format Name", UADEHelper::instance()->tag("formatname"));
        ep << MetaDataItem("Player Name", UADEHelper::instance()->tag("playername"));
    }
    return ep;
}
