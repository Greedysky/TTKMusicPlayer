#include "psfmetadatamodel.h"

PSFMetaDataModel::PSFMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    m_helper = new PSFHelper(path);
    m_tags << new PSFFileTagModel(m_helper);
}

PSFMetaDataModel::~PSFMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
    delete m_helper;
}

QList<TagModel* > PSFMetaDataModel::tags() const
{
    return m_tags;
}

QList<MetaDataItem> PSFMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;

    if(m_helper->initialize())
    {
        const QMap<QString, QString> &data = m_helper->readMetaTags();
        for(const QString &key : data.keys())
        {
            ep << MetaDataItem(key, data[key]);
        }
    }

    return ep;
}


PSFFileTagModel::PSFFileTagModel(PSFHelper* psf)
    : TagModel(),
      m_helper(psf)
{

}

PSFFileTagModel::~PSFFileTagModel()
{

}

QString PSFFileTagModel::name() const
{
    return "ID3v1";
}

QList<Qmmp::MetaData> PSFFileTagModel::keys() const
{
    QList<Qmmp::MetaData> list = TagModel::keys();
    list.removeAll(Qmmp::COMPOSER);
    list.removeAll(Qmmp::ALBUMARTIST);
    list.removeAll(Qmmp::DISCNUMBER);
    list.removeAll(Qmmp::GENRE);
    list.removeAll(Qmmp::COMMENT);
    list.removeAll(Qmmp::TRACK);
    return list;
}

QString PSFFileTagModel::value(Qmmp::MetaData key) const
{
    if(m_helper && m_helper->initialize())
    {
        m_helper->readMetaTags();
        switch((int) key)
        {
        case Qmmp::TITLE: return m_helper->title();
        case Qmmp::ARTIST: return m_helper->artist();
        case Qmmp::ALBUM: return m_helper->album();
        case Qmmp::YEAR: return m_helper->year();
        }
    }
    return QString();
}

void PSFFileTagModel::setValue(Qmmp::MetaData , const QString &)
{
}
