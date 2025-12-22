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
    m_ep << MetaDataItem(tr("SubSong size"), helper->subsongs());
    m_ep << MetaDataItem(tr("Pattern channel size"), helper->patternChannels());
    m_ep << MetaDataItem(tr("Order size"), helper->orders());
    m_ep << MetaDataItem(tr("Pattern size"), helper->patterns());
    m_ep << MetaDataItem(tr("Sample size"), helper->samples());
    m_ep << MetaDataItem(tr("Instrument size"), helper->instruments());

    QString value;
    for(int i = 0; i < helper->subsongs(); ++i)
    {
        const QString &v = helper->subsong(i);
        if(!v.isEmpty())
        {
            value += v + "\n";
        }
    }

    if(!value.isEmpty())
    {
        value.chop(1); // last \n
        m_desc << MetaDataItem(tr("Titles"), value);
        value.clear();
    }

    for(int i = 0; i < helper->patternChannels(); ++i)
    {
        const QString &v = helper->patternChannel(i);
        if(!v.isEmpty())
        {
            value += v + "\n";
        }
    }

    if(!value.isEmpty())
    {
        value.chop(1); // last \n
        m_desc << MetaDataItem(tr("Channels"), value);
        value.clear();
    }

    for(int i = 0; i < helper->orders(); ++i)
    {
        const QString &v = helper->order(i);
        if(!v.isEmpty())
        {
            value += v + "\n";
        }
    }

    if(!value.isEmpty())
    {
        value.chop(1); // last \n
        m_desc << MetaDataItem(tr("Orders"), value);
        value.clear();
    }

    for(int i = 0; i < helper->patterns(); ++i)
    {
        const QString &v = helper->pattern(i);
        if(!v.isEmpty())
        {
            value += v + "\n";
        }
    }

    if(!value.isEmpty())
    {
        value.chop(1); // last \n
        m_desc << MetaDataItem(tr("Patterns"), value);
        value.clear();
    }

    for(int i = 0; i < helper->samples(); ++i)
    {
        const QString &v = helper->sample(i);
        if(!v.isEmpty())
        {
            value += v + "\n";
        }
    }

    if(!value.isEmpty())
    {
        value.chop(1); // last \n
        m_desc << MetaDataItem(tr("Samples"), value);
        value.clear();
    }

    for(int i = 0; i < helper->instruments(); ++i)
    {
        const QString &v = helper->instrument(i);
        if(!v.isEmpty())
        {
            value += v + "\n";
        }
    }

    if(!value.isEmpty())
    {
        value.chop(1); // last \n
        m_desc << MetaDataItem(tr("Instruments"), value);
    }
}

QList<MetaDataItem> OpenMPTMetaDataModel::extraProperties() const
{
    return m_ep;
}

QList<MetaDataItem> OpenMPTMetaDataModel::descriptions() const
{
    return m_desc;
}
