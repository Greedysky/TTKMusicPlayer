#include "openmptmetadatamodel.h"
#include "openmpthelper.h"

#include <QFile>

OpenMPTMetaDataModel::OpenMPTMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
  QFile file(path);
  if(file.open(QIODevice::ReadOnly))
  {
      OpenMPTHelper helper(&file);
      if(helper.initialize())
      {
          fillInExtraProperties(&helper);
          fillInDescriptions(&helper);
          file.close();
      }
  }
}

void OpenMPTMetaDataModel::fillInExtraProperties(OpenMPTHelper *helper)
{
    QString text;
    for(const QString &s : helper->instruments())
    {
        if(s.isEmpty())
        {
            continue;
        }
        text += s + "\n";
    }
    m_desc << MetaDataItem(tr("Instruments"), text);

    text.clear();
    for(const QString &s : helper->samples())
    {
        if(s.isEmpty())
        {
            continue;
        }
        text += s + "\n";
    }

    m_desc << MetaDataItem(tr("Samples"), text);
    m_desc << MetaDataItem(tr("Comment"), helper->comment());
}

void OpenMPTMetaDataModel::fillInDescriptions(OpenMPTHelper *helper)
{
    m_ap << MetaDataItem(tr("Patterns"), QString::number(helper->patternCount()));
    m_ap << MetaDataItem(tr("Channels"), QString::number(helper->channelCount()));
    m_ap << MetaDataItem(tr("Instruments"), QString::number(helper->instrumentCount()));
    m_ap << MetaDataItem(tr("Samples"), QString::number(helper->sampleCount()));
}

QList<MetaDataItem> OpenMPTMetaDataModel::extraProperties() const
{
    return m_ap;
}

QList<MetaDataItem> OpenMPTMetaDataModel::descriptions() const
{
    return m_desc;
}
