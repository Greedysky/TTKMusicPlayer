#include "optimfrogmetadatamodel.h"
#include "optimfroghelper.h"

OptimFROGMetaDataModel::OptimFROGMetaDataModel(const QString &path)
    : MetaDataModel(true),
      m_path(path)
{

}

QList<MetaDataItem> OptimFROGMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;
    QFile file(m_path);
    if(file.open(QIODevice::ReadOnly))
    {
        OptimFROGHelper helper(&file);
        if(!helper.initialize())
        {
            return ep;
        }

        ep << MetaDataItem(tr("Version"), helper.version());
        ep << MetaDataItem(tr("Compression ratio"), helper.compression());
        file.close();
    }
    return ep;
}
