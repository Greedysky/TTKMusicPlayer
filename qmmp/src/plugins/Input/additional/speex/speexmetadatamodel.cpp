#include "speexmetadatamodel.h"
#include "speexhelper.h"

SpeexMetaDataModel::SpeexMetaDataModel(const QString &path)
    : MetaDataModel(true),
      m_path(path)
{

}

QList<MetaDataItem> SpeexMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;

    QFile file(m_path);
    if(file.open(QIODevice::ReadOnly))
    {
        SpeexHelper helper(&file);
        if(!helper.initialize())
        {
            return ep;
        }

        for(int i = 0; i < helper.stream_get_tagcount(); ++i)
        {
            const speextags* tag = helper.stream_get_tags()[i];
            if(tag->item)
            {
                ep << MetaDataItem(tag->item, tag->value ? tag->value : QString());
            }
        }
    }

    return ep;
}
