#include "asapmetadatamodel.h"

AsapMetaDataModel::AsapMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    m_asap = new AsapHelper(path);
    m_tags << new AsapFileTagModel(m_asap);
}

AsapMetaDataModel::~AsapMetaDataModel()
{
    while(!m_tags.isEmpty())
    {
        delete m_tags.takeFirst();
    }
    delete m_asap;
}

QList<TagModel* > AsapMetaDataModel::tags() const
{
    return m_tags;
}

QList<MetaDataItem> AsapMetaDataModel::extraProperties() const
{
    QList<MetaDataItem> ep;

    if(m_asap->initialize())
    {
        const QMap<QString, QString> &data = m_asap->readMetaTags();
        for(const QString &key : data.keys())
        {
            ep << MetaDataItem(key, data[key]);
        }
    }

    return ep;
}


AsapFileTagModel::AsapFileTagModel(AsapHelper* asap)
    : TagModel(),
      m_asap(asap)
{

}

AsapFileTagModel::~AsapFileTagModel()
{

}

QString AsapFileTagModel::name() const
{
    return "ID3v1";
}

QList<Qmmp::MetaData> AsapFileTagModel::keys() const
{
    QList<Qmmp::MetaData> list = TagModel::keys();
    list.removeAll(Qmmp::COMPOSER);
    list.removeAll(Qmmp::ALBUMARTIST);
    list.removeAll(Qmmp::DISCNUMBER);
    list.removeAll(Qmmp::GENRE);
    list.removeAll(Qmmp::COMMENT);
    list.removeAll(Qmmp::TRACK);
    list.removeAll(Qmmp::ALBUM);
    return list;
}

QString AsapFileTagModel::value(Qmmp::MetaData key) const
{
    if(m_asap && m_asap->initialize())
    {
        m_asap->readMetaTags();
        switch((int) key)
        {
        case Qmmp::TITLE: return m_asap->title();
        case Qmmp::ARTIST: return m_asap->artist();
        case Qmmp::YEAR: return m_asap->year();
        }
    }
    return QString();
}

void AsapFileTagModel::setValue(Qmmp::MetaData , const QString &)
{
}
