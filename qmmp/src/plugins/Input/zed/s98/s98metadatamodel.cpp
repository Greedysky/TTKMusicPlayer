#include "s98metadatamodel.h"
#include "s98helper.h"

S98MetaDataModel::S98MetaDataModel(const QString &path)
    : MetaDataModel(true),
      m_path(path)
{

}

QList<MetaDataItem> S98MetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;
    S98Helper helper(m_path);
    if(!(helper.initialize() && helper.hasMetaData()))
    {
        return ep;
    }

    ep << MetaDataItem("Game", helper.metaData("game"));
    ep << MetaDataItem("System", helper.metaData("system"));
    ep << MetaDataItem("Copyright", helper.metaData("copyright"));
    return ep;
}
