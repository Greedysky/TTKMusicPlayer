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
    if(!(helper.initialize() && helper.hasTags()))
    {
        return ep;
    }

    ep << MetaDataItem("Module MD5", helper.tag("modulemd5"));
    ep << MetaDataItem("Module Name", helper.tag("modulename"));
    ep << MetaDataItem("Module Path", helper.tag("modulepath"));
    ep << MetaDataItem("Format", helper.tag("format"));
    ep << MetaDataItem("Format Name", helper.tag("formatname"));
    ep << MetaDataItem("Player Name", helper.tag("playername"));
    return ep;
}
