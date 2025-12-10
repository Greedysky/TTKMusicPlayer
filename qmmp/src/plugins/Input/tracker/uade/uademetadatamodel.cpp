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

    ep << MetaDataItem(tr("Module MD5"), helper.metaData("modulemd5"));
    ep << MetaDataItem(tr("Module name"), helper.metaData("modulename"));
    ep << MetaDataItem(tr("Module path"), helper.metaData("modulepath"));
    ep << MetaDataItem(tr("Format"), helper.metaData("format"));
    ep << MetaDataItem(tr("Format name"), helper.metaData("formatname"));
    ep << MetaDataItem(tr("Player name"), helper.metaData("playername"));
    return ep;
}
