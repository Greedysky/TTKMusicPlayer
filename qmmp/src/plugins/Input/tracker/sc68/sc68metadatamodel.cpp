#include "sc68metadatamodel.h"
#include "sc68helper.h"

SC68MetaDataModel::SC68MetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    SC68Helper helper(path);
    if(helper.initialize())
    {
        fillProperties(&helper);
    }
}

void SC68MetaDataModel::fillProperties(SC68Helper *helper)
{
    m_ep << MetaDataItem(tr("Format"), helper->format());
    m_ep << MetaDataItem(tr("Ripper"), helper->ripper());
    m_ep << MetaDataItem(tr("Converter"), helper->converter());
}

QList<MetaDataItem> SC68MetaDataModel::extraProperties() const
{
    return m_ep;
}
