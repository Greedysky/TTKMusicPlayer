#include "uademetadatamodel.h"
#include "uadehelper.h"

UADEMetaDataModel::UADEMetaDataModel(const QString &path)
    : MetaDataModel(true),
      m_path(path)
{

}

QList<MetaDataItem> UADEMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;
    UADEHelper helper(m_path);
    if(!(helper.initialize() && helper.hasMetaData()))
    {
        return ep;
    }

    ep << MetaDataItem("Module MD5", helper.metaData("modulemd5"));
    ep << MetaDataItem("Module Name", helper.metaData("modulename"));
    ep << MetaDataItem("Module Path", helper.metaData("modulepath"));
    ep << MetaDataItem("Format", helper.metaData("format"));
    ep << MetaDataItem("Format Name", helper.metaData("formatname"));
    ep << MetaDataItem("Player Name", helper.metaData("playername"));
    return ep;
}
