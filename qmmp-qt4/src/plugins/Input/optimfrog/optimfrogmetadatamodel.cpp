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
        OptimFROGWrap wrap(&file);
        if(!wrap.initialize())
        {
            return ep;
        }

        ep << MetaDataItem(tr("Version"), QString::number(wrap.version()));
        ep << MetaDataItem(tr("Compression ratio"), QString::number(wrap.compression()));
    }

    return ep;
}
