#include "trackinfo.h"

TrackInfo::TrackInfo(const QString &path)
    : m_path(path)
{

}

TrackInfo::TrackInfo(const TrackInfo &other)
{
    *this = other;
}

TrackInfo &TrackInfo::operator=(const TrackInfo &info)
{
    setDuration(info.duration());
    setValues(info.metaData());
    setValues(info.properties());
    setValues(info.replayGainInfo());
    setPath(info.path());
    return *this;
}

bool TrackInfo::operator==(const TrackInfo &info) const
{
    return m_duration == info.duration() &&
            m_path == info.path() &&
            m_metaData == info.metaData() &&
            m_properties == info.properties() &&
            m_replayGainInfo == info.replayGainInfo() &&
            m_parts == info.parts();
}

bool TrackInfo::operator!=(const TrackInfo &info) const
{
    return !operator==(info);
}

qint64 TrackInfo::duration() const
{
    return m_duration;
}

bool TrackInfo::isEmpty() const
{
    return m_metaData.isEmpty() && m_properties.isEmpty() && m_replayGainInfo.isEmpty() && m_path.isEmpty();
}

const QString &TrackInfo::path() const
{
    return m_path;
}

const QString TrackInfo::value(Qmmp::MetaData key) const
{
    return m_metaData.value(key);
}

const QString TrackInfo::value(Qmmp::TrackProperty key) const
{
    return m_properties.value(key);
}

double TrackInfo::value(Qmmp::ReplayGainKey key) const
{
    return m_replayGainInfo.value(key);
}

const QMap<Qmmp::MetaData, QString> &TrackInfo::metaData() const
{
    return m_metaData;
}

const QMap<Qmmp::TrackProperty, QString> &TrackInfo::properties() const
{
    return m_properties;
}

const QMap<Qmmp::ReplayGainKey, double> &TrackInfo::replayGainInfo() const
{
    return m_replayGainInfo;
}

TrackInfo::Parts TrackInfo::parts() const
{
    return m_parts;
}

void TrackInfo::setDuration(qint64 duration)
{
    m_duration = duration;
}

void TrackInfo::setValue(Qmmp::MetaData key, const QVariant &value)
{
    QString strValue = value.toString().trimmed();
    if(strValue.isEmpty() || strValue == "0")
        m_metaData.remove(key);
    else
        m_metaData[key] = strValue;
    m_metaData.isEmpty() ? (m_parts &= ~MetaData) : (m_parts |= MetaData);
}

void TrackInfo::setValue(Qmmp::MetaData key, const char *value)
{
    setValue(key, QString::fromUtf8(value));
}

void TrackInfo::setValue(Qmmp::TrackProperty key, const QVariant &value)
{
    QString strValue = value.toString();
    if(strValue.isEmpty() || strValue == "0")
        m_properties.remove(key);
    else
        m_properties[key] = strValue;
    m_properties.isEmpty() ? (m_parts &= ~Properties) : (m_parts |= Properties);
}

void TrackInfo::setValue(Qmmp::TrackProperty key, const char *value)
{
    setValue(key, QString::fromUtf8(value));
}

void TrackInfo::setValue(Qmmp::ReplayGainKey key, double value)
{
    if(qFuzzyIsNull(value))
        m_replayGainInfo.remove(key);
    else
        m_replayGainInfo[key] = value;
    m_replayGainInfo.isEmpty() ? (m_parts &= ~ReplayGainInfo) : (m_parts |= ReplayGainInfo);
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
   m_metaData.clear();
   updateValues(metaData);
}

void TrackInfo::setValues(const QMap<Qmmp::TrackProperty, QString> &properties)
{
    m_properties.clear();
    updateValues(properties);
}

void TrackInfo::setValues(const QMap<Qmmp::ReplayGainKey, double> &replayGainInfo)
{
    m_replayGainInfo.clear();
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
    m_path = path;
}

void TrackInfo::clear(Parts parts)
{
    if(parts & MetaData)
        m_metaData.clear();
    if(parts & Properties)
        m_properties.clear();
    if(parts & ReplayGainInfo)
        m_replayGainInfo.clear();
    m_parts &= ~parts;
}

void TrackInfo::clear()
{
    clear(AllParts);
    m_path.clear();
    m_duration = 0;
}

QString TrackInfo::pathFromUrl(const QString &url, int *track)
{
    QString path = url;
    int index = path.indexOf("://");
    if(index > 0)
    {
        path.remove(0, index + 3);
    }

    const QString &trackStr = path.section("#", -1);
    bool ok = false;
    int t = trackStr.toInt(&ok);
    if(ok)
    {
        if(track)
        {
            *track = t;
        }

        index = path.lastIndexOf("#");
        path.remove(index, path.size() - index);
    }
    else if(track)
    {
        *track = -1;
    }
    return path;
}
