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

QList<TagModel*> AsapMetaDataModel::tags() const
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
        const QMap<Qmmp::MetaData, QString> metaData(m_helper->readMetaData());
        switch((int) key)
        {
        case Qmmp::TITLE: return metaData.value(Qmmp::TITLE);
        case Qmmp::ARTIST: return metaData.value(Qmmp::ARTIST);
        case Qmmp::YEAR: return metaData.value(Qmmp::YEAR);
        }
    }
    return QString();
}

void AsapFileTagModel::setValue(Qmmp::MetaData , const QString &)
{
}
