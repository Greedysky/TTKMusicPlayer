#include "stsoundmetadatamodel.h"
#include "stsoundhelper.h"

StSoundMetaDataModel::StSoundMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    StSoundHelper helper(path);
    if(helper.initialize())
    {
        fillProperties(&helper);
    }
}

void StSoundMetaDataModel::fillProperties(StSoundHelper *helper)
{
    m_ep << MetaDataItem(tr("Song type"), helper->type());
    m_ep << MetaDataItem(tr("Song player"), helper->player());
    m_ep << MetaDataItem(tr("Frames"), helper->frames());
    m_ep << MetaDataItem(tr("Stream inc"), helper->streamInc());
}

QList<MetaDataItem> StSoundMetaDataModel::extraProperties() const
{
    return m_ep;
}
