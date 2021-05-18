#include "asapmetadatamodel.h"

AsapMetaDataModel::AsapMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    m_helper = new AsapHelper(path);
    m_tags << new AsapFileTagModel(m_helper);
}

AsapMetaDataModel::~AsapMetaDataModel()
{
    while(!m_tags.isEmpty())
    {
        delete m_tags.takeFirst();
    }
    delete m_helper;
}

QList<TagModel* > AsapMetaDataModel::tags() const
{
    return m_tags;
}


AsapFileTagModel::AsapFileTagModel(AsapHelper* asap)
    : TagModel(),
      m_helper(asap)
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
    if(m_helper && m_helper->initialize())
    {
        m_helper->readMetaTags();
        switch((int) key)
        {
        case Qmmp::TITLE: return m_helper->title();
        case Qmmp::ARTIST: return m_helper->artist();
        case Qmmp::YEAR: return m_helper->year();
        }
    }
    return QString();
}

void AsapFileTagModel::setValue(Qmmp::MetaData , const QString &)
{
}
