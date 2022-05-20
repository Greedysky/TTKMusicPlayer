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
    if(!(helper.initialize() && helper.hasTags()))
    {
        return ep;
    }

    ep << MetaDataItem("Game", helper.tag("game"));
    ep << MetaDataItem("Fade", helper.tag("fade"));
    ep << MetaDataItem("Ripper", helper.tag("ripper"));
    ep << MetaDataItem("Copyright", helper.tag("copyright"));
    return ep;
}
