#include "psfmetadatamodel.h"
#include "psfhelper.h"

PSFMetaDataModel::PSFMetaDataModel(const QString &path)
    : MetaDataModel(true),
      m_path(path)
{

}

QList<MetaDataItem> PSFMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;
    PSFHelper helper(m_path);
    if(!(helper.initialize() && helper.hasMetaData()))
    {
        return ep;
    }

    ep << MetaDataItem("Game", helper.metaData("game"));
    ep << MetaDataItem("Fade", helper.metaData("fade"));
    ep << MetaDataItem("Ripper", helper.metaData("ripper"));
    ep << MetaDataItem("Copyright", helper.metaData("copyright"));
    return ep;
}
