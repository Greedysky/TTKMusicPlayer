#include "openmptmetadatamodel.h"
#include "openmpthelper.h"

OpenMPTMetaDataModel::OpenMPTMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
  QFile file(path);
  if(file.open(QIODevice::ReadOnly))
  {
      OpenMPTHelper helper(&file);
      if(helper.initialize())
      {
          fillProperties(&helper);
      }

      file.close();
  }
}

void OpenMPTMetaDataModel::fillProperties(OpenMPTHelper *helper)
{
    m_ep << MetaDataItem(tr("Patterns"), QString::number(helper->patternCount()));
    m_ep << MetaDataItem(tr("Channels"), QString::number(helper->channelCount()));
    m_ep << MetaDataItem(tr("Samples"), QString::number(helper->sampleCount()));
    m_ep << MetaDataItem(tr("Instruments"), QString::number(helper->instrumentCount()));

    m_desc << MetaDataItem(tr("Samples"), helper->samples());
    m_desc << MetaDataItem(tr("Instruments"), helper->instruments());
}

QList<MetaDataItem> OpenMPTMetaDataModel::extraProperties() const
{
    return m_ep;
}

QList<MetaDataItem> OpenMPTMetaDataModel::descriptions() const
{
    return m_desc;
}
