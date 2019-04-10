#include "optimfrogmetadatamodel.h"
#include "optimfrogwrap.h"

OptimFROGMetaDataModel::OptimFROGMetaDataModel(const QString &path) : MetaDataModel(true)
{
    m_path = path;
}

QList<MetaDataItem> OptimFROGMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;

    QFile file(m_path);
    if(file.open(QIODevice::ReadOnly))
    {
        try
        {
            OptimFROGWrap frog(&file);
            ep << MetaDataItem(tr("Version"), QString::number(frog.version()));
            ep << MetaDataItem(tr("Compression ratio"), QString::number(frog.compression()));
        }
        catch(const OptimFROGWrap::InvalidFile &)
        {
        }
    }

    return ep;
}
