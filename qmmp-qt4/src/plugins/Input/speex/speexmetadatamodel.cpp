#include "speexmetadatamodel.h"
#include "speexhelper.h"

SpeexMetaDataModel::SpeexMetaDataModel(const QString &path) : MetaDataModel(true)
{
    m_path = path;
}

QList<MetaDataItem> SpeexMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;

    QFile file(m_path);
    if(file.open(QIODevice::ReadOnly))
    {
        SpeexHelper wrap(&file);
        if(!wrap.initialize())
        {
            return ep;
        }

        for(int i = 0; i < wrap.stream_get_tagcount(); ++i)
        {
            const speextags* tag = wrap.stream_get_tags()[i];
            if(tag->item)
            {
                ep << MetaDataItem(tag->item, tag->value ? tag->value : QString());
            }
        }
    }

    return ep;
}
