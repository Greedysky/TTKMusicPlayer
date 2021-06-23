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

QList<TagModel*> PSFMetaDataModel::tags() const
{
    return m_tags;
}

QList<MetaDataItem> PSFMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;

    if(m_helper->initialize())
    {
        const QMap<Qmmp::MetaData, QString> &data = m_helper->readMetaData();
        ep << MetaDataItem("Copyright", data[Qmmp::COMPOSER]);
        ep << MetaDataItem("Comment", data[Qmmp::COMMENT]);
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
        const QMap<Qmmp::MetaData, QString> &data = m_helper->readMetaData();
        switch((int) key)
        {
        case Qmmp::TITLE: return data.value(Qmmp::TITLE);
        case Qmmp::ARTIST: return data.value(Qmmp::ARTIST);
        case Qmmp::ALBUM: return data.value(Qmmp::ALBUM);
        case Qmmp::YEAR: return data.value(Qmmp::YEAR);
        }
    }
    return QString();
}

void PSFFileTagModel::setValue(Qmmp::MetaData , const QString &)
{
}
