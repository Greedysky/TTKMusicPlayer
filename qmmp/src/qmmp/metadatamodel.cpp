#include "metadatamodel.h"

class MetaDataItemPrivate
{
public:
    QString name, suffix;
    QVariant value;

};

class MetaDataModelPrivate
{
public:
    bool readOnly;
    MetaDataModel::DialogHints dialogHints;

};


MetaDataItem::MetaDataItem(const QString &name, const QVariant &value, const QString &suffix)
    : d(new MetaDataItemPrivate)
{
    d->name = name;
    d->suffix = suffix;
    d->value = value;
}

MetaDataItem::MetaDataItem(const MetaDataItem &other)
    : d(new MetaDataItemPrivate)
{
    operator=(other);
}

MetaDataItem::MetaDataItem(MetaDataItem &&other) noexcept
    : d(nullptr)
{
    std::swap(d, other.d);
}

MetaDataItem::~MetaDataItem()
{
    delete d;
}

MetaDataItem &MetaDataItem::operator=(const MetaDataItem &other)
{
    d->name = other.d->name;
    d->value = other.d->value;
    d->suffix = other.d->suffix;
    return *this;
}

MetaDataItem &MetaDataItem::operator=(MetaDataItem &&other) noexcept
{
    std::swap(d, other.d);
    return *this;
}

const QString &MetaDataItem::name() const
{
    return d->name;
}

void MetaDataItem::setName(const QString &name)
{
    d->name = name;
}

const QVariant &MetaDataItem::value() const
{
    return d->value;
}

void MetaDataItem::setValue(const QVariant &value)
{
    d->value = value;
}

const QString &MetaDataItem::suffix() const
{
    return d->suffix;
}

void MetaDataItem::setSuffix(const QString &suffix)
{
    d->suffix = suffix;
}


MetaDataModel::MetaDataModel(bool readOnly, DialogHints hints)
    : d(new MetaDataModelPrivate)
{
    d->readOnly = readOnly;
    d->dialogHints = hints;
}

MetaDataModel::~MetaDataModel()
{
    delete d;
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
    return d->readOnly;
}

const MetaDataModel::DialogHints &MetaDataModel::dialogHints() const
{
    return d->dialogHints;
}

void MetaDataModel::setDialogHints(const DialogHints &hints)
{
    d->dialogHints = hints;
}

void MetaDataModel::setReadOnly(bool readOnly)
{
    d->readOnly = readOnly;
}
