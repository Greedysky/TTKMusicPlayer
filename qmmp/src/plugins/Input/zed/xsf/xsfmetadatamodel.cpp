#include "xsfmetadatamodel.h"
#include "xsfhelper.h"

XSFMetaDataModel::XSFMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    XSFHelper helper(path, true);
    if(helper.initialize())
    {
        fillProperties(&helper);
    }
}

void XSFMetaDataModel::fillProperties(XSFHelper *helper)
{
    m_ep << MetaDataItem(tr("Song duration"), helper->tag("song_ms"));
    m_ep << MetaDataItem(tr("Fade duration"), helper->tag("fade_ms"));
    m_ep << MetaDataItem(tr("Comment"), helper->tag("comment"));
    m_ep << MetaDataItem(tr("Copyright"), helper->tag("copyright"));
}

QList<MetaDataItem> XSFMetaDataModel::extraProperties() const
{
    return m_ep;
}
