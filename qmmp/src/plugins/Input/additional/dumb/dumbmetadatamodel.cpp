#include "dumbmetadatamodel.h"
#include "dumbhelper.h"

DumbMetaDataModel::DumbMetaDataModel(const QString &path)
    : MetaDataModel(true),
      m_path(path)
{

}

QList<MetaDataItem> DumbMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;

    DumbHelper helper(m_path);
    if(helper.initialize())
    {
        const QVariantMap &data = helper.readMetaTags();
        for(const QString &key : data.keys())
        {
            ep << MetaDataItem(key, data[key]);
        }
    }

    return ep;
}
