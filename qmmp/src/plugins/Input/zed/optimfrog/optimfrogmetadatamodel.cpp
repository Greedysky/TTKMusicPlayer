#include "optimfrogmetadatamodel.h"
#include "optimfroghelper.h"

OptimFROGMetaDataModel::OptimFROGMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        OptimFROGHelper helper(&file);
        if(helper.initialize())
        {
            fillProperties(&helper);
        }

        file.close();
    }
}

void OptimFROGMetaDataModel::fillProperties(OptimFROGHelper *helper)
{
    m_ep << MetaDataItem(tr("Version"), helper->version());
    m_ep << MetaDataItem(tr("Compression ratio"), helper->compression());
}

QList<MetaDataItem> OptimFROGMetaDataModel::extraProperties() const
{
    return m_ep;
}
