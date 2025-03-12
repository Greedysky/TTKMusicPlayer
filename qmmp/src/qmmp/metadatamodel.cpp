#include "metadatamodel.h"

MetaDataItem::MetaDataItem(const QString &name, const QVariant &value, const QString &suffix)
    : m_name(name),
      m_suffix(suffix),
      m_value(value)
{

}

const QString &MetaDataItem::name() const
{
    return m_name;
}

void MetaDataItem::setName(const QString &name)
{
    m_name = name;
}

const QVariant &MetaDataItem::value() const
{
    return m_value;
}

void MetaDataItem::setValue(const QString &value)
{
    m_value = value;
}

const QString &MetaDataItem::suffix() const
{
    return m_suffix;
}

void MetaDataItem::setSuffix(const QString &suffix)
{
    m_suffix = suffix;
}

MetaDataModel::MetaDataModel(bool readOnly, DialogHints hints)
    : m_readOnly(readOnly),
      m_dialogHints(hints)
{

}

QList<MetaDataItem> MetaDataModel::extraProperties() const
{
    return QList<MetaDataItem>();
}

QList<MetaDataItem> MetaDataModel::descriptions() const
{
    return QList<MetaDataItem>();
}

QList<TagModel*> MetaDataModel::tags() const
{
    return QList<TagModel*>();
}

QImage MetaDataModel::cover() const
{
    return QImage();
}

void MetaDataModel::setCover(const QImage &img)
{
    Q_UNUSED(img);
}

void MetaDataModel::removeCover()
{

}

QString MetaDataModel::coverPath() const
{
    return QString();
}

QString MetaDataModel::cue() const
{
    return QString();
}

void MetaDataModel::setCue(const QString &content)
{
    Q_UNUSED(content);
}

void MetaDataModel::removeCue()
{

}

QString MetaDataModel::lyrics() const
{
    return QString();
}

bool MetaDataModel::isReadOnly() const
{
    return m_readOnly;
}

const MetaDataModel::DialogHints &MetaDataModel::dialogHints() const
{
    return m_dialogHints;
}

void MetaDataModel::setDialogHints(const DialogHints &hints)
{
    m_dialogHints = hints;
}

void MetaDataModel::setReadOnly(bool readOnly)
{
    m_readOnly = readOnly;
}
