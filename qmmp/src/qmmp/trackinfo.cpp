#include "trackinfo.h"

class TrackInfoPrivate : public QSharedData
{
public:
    TrackInfoPrivate(const QString &path)
        : QSharedData(),
          path(path)
    {

    }

    void clear(TrackInfo::Parts parts)
    {
        if(parts & TrackInfo::MetaData)
            metaData.clear();
        if(parts & TrackInfo::Properties)
            properties.clear();
        if(parts & TrackInfo::ReplayGainInfo)
            replayGainInfo.clear();
        parts &= ~parts;
    }

    QMap<Qmmp::MetaData, QString> metaData;
    QMap<Qmmp::TrackProperty, QString> properties;
    QMap<Qmmp::ReplayGainKey, double> replayGainInfo;
    TrackInfo::Parts parts = TrackInfo::Parts();
    QString path;
    qint64 duration = 0;

};


TrackInfo::TrackInfo(const QString &path)
    : d(new TrackInfoPrivate(path))
{

}

TrackInfo::TrackInfo(const TrackInfo &other)
    : d(other.d)
{

}

TrackInfo::TrackInfo(TrackInfo &&other) noexcept
{
    swap(other);
}

TrackInfo::~TrackInfo()
{

}

TrackInfo &TrackInfo::operator=(const TrackInfo &other)
{
    d = other.d;
    return *this;
}

TrackInfo &TrackInfo::operator=(TrackInfo &&other) noexcept
{
    swap(other);
    return *this;
}

bool TrackInfo::operator==(const TrackInfo &other) const
{
    return d->duration == other.duration() &&
           d->path == other.path() &&
           d->metaData == other.metaData () &&
           d->properties == other.properties() &&
           d->replayGainInfo == other.replayGainInfo() &&
           d->parts == other.parts();
}

bool TrackInfo::operator!=(const TrackInfo &other) const
{
    return !operator==(other);
}

qint64 TrackInfo::duration() const
{
    return d->duration;
}

bool TrackInfo::isEmpty() const
{
    return d->metaData.isEmpty() && d->properties.isEmpty() && d->replayGainInfo.isEmpty() && d->path.isEmpty();
}

const QString &TrackInfo::path() const
{
    return d->path;
}

QString TrackInfo::value(Qmmp::MetaData key) const
{
    return d->metaData.value(key);
}

 QString TrackInfo::value(Qmmp::TrackProperty key) const
{
    return d->properties.value(key);
}

double TrackInfo::value(Qmmp::ReplayGainKey key) const
{
    return d->replayGainInfo.value(key);
}

const QMap<Qmmp::MetaData, QString> &TrackInfo::metaData() const
{
    return d->metaData;
}

const QMap<Qmmp::TrackProperty, QString> &TrackInfo::properties() const
{
    return d->properties;
}

const QMap<Qmmp::ReplayGainKey, double> &TrackInfo::replayGainInfo() const
{
    return d->replayGainInfo;
}

TrackInfo::Parts TrackInfo::parts() const
{
    return d->parts;
}

void TrackInfo::setDuration(qint64 duration)
{
    d->duration = duration;
}

void TrackInfo::setValue(Qmmp::MetaData key, const QVariant &value)
{
    QString strValue = value.toString().trimmed();
    if(strValue.isEmpty() || strValue == "0")
        d->metaData.remove(key);
    else
        d->metaData[key] = strValue;
    d->metaData.isEmpty() ? (d->parts &= ~MetaData) : (d->parts |= MetaData);
}

void TrackInfo::setValue(Qmmp::MetaData key, const char *value)
{
    setValue(key, QString::fromUtf8(value));
}

void TrackInfo::setValue(Qmmp::TrackProperty key, const QVariant &value)
{
    QString strValue = value.toString();
    if(strValue.isEmpty() || strValue == "0")
        d->properties.remove(key);
    else
        d->properties[key] = strValue;
    d->properties.isEmpty() ? (d->parts &= ~Properties) : (d->parts |= Properties);
}

void TrackInfo::setValue(Qmmp::TrackProperty key, const char *value)
{
    setValue(key, QString::fromUtf8(value));
}

void TrackInfo::setValue(Qmmp::ReplayGainKey key, double value)
{
    if(qFuzzyIsNull(value))
        d->replayGainInfo.remove(key);
    else
        d->replayGainInfo[key] = value;
    d->replayGainInfo.isEmpty() ? (d->parts &= ~ReplayGainInfo) : (d->parts |= ReplayGainInfo);
}

void TrackInfo::setValue(Qmmp::ReplayGainKey key, const QString &value)
{
    QString str = value;
    str.remove(RegularExpression("[\\sA-Za-z]"));
    str = str.trimmed();
    bool ok = false;
    double v = str.toDouble(&ok);
    if(ok)
        setValue(key, v);
}

void TrackInfo::setValues(const QMap<Qmmp::MetaData, QString> &metaData)
{
    updateValues(metaData);
}

void TrackInfo::setValues(const QMap<Qmmp::TrackProperty, QString> &properties)
{
    updateValues(properties);
}

void TrackInfo::setValues(const QMap<Qmmp::ReplayGainKey, double> &replayGainInfo)
{
    updateValues(replayGainInfo);
}

void TrackInfo::updateValues(const QMap<Qmmp::MetaData, QString> &metaData)
{
    for(auto it = metaData.begin(); it != metaData.end(); ++it)
    {
        setValue(it.key(), it.value());
    }
}

void TrackInfo::updateValues(const QMap<Qmmp::TrackProperty, QString> &properties)
{
    for(auto it = properties.begin(); it != properties.end(); ++it)
    {
        setValue(it.key(), it.value());
    }
}

void TrackInfo::updateValues(const QMap<Qmmp::ReplayGainKey, double> &replayGainInfo)
{
    for(auto it = replayGainInfo.begin(); it != replayGainInfo.end(); ++it)
    {
        setValue(it.key(), it.value());
    }
}

void TrackInfo::setPath(const QString &path)
{
    d->path = path;
}

void TrackInfo::clear(Parts parts)
{
    d->clear(parts);
}

void TrackInfo::clear()
{
    d->clear(AllParts);
    d->path.clear();
    d->duration = 0;
}

void TrackInfo::swap(TrackInfo &other)
{
    d.swap(other.d);
}

QString TrackInfo::pathFromUrl(const QString &url, int *track)
{
    if(track)
    {
        *track = -1;
    }

    const int prefix = url.indexOf("://");
    if(prefix < 0)
    {
        return url;
    }

    const int suffix = url.lastIndexOf(QLatin1Char('#'));
    if(suffix < 0)
    {
        return url;
    }

    const QString &path = url.mid(prefix + 3, suffix - prefix - 3);
    const QString &trackStr = url.mid(suffix + 1);
    if(track)
    {
        *track = trackStr.toInt();
    }
    return path;
}
