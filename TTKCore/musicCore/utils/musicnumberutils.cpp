#include "musicnumberutils.h"
#include "musicnumberdefine.h"

#include <QStringList>

QString MusicUtils::Number::size2Number(qint64 size)
{
    if( size < MH_KB2B)
    {
        return QString("%1").arg(size);
    }
    else if( MH_KB2B <= size && size < MH_MB2B)
    {
        return QString("%1").arg((qint64)(size*1.0/MH_KB2B*100)/100.0);
    }
    else if( MH_MB2B <= size && size < MH_GB2B)
    {
        return QString("%1").arg((qint64)(size*1.0/MH_MB2B*100)/100.0);
    }
    else if( MH_GB2B <= size && size < MH_TB2B)
    {
        return QString("%1").arg((qint64)(size*1.0/MH_GB2B*100)/100.0);
    }
    else
    {
        return QString();
    }
}

QString MusicUtils::Number::size2NumberInt(qint64 size)
{
    QString label = size2Number(size);
    if(label.contains("."))
    {
        label = label.split(".").front();
    }
    return label;
}

QString MusicUtils::Number::size2Label(qint64 size)
{
    QString label = size2Number(size);
    if( size < MH_KB2B)
    {
        return QString("%1 B").arg(label);
    }
    else if( MH_KB2B <= size && size < MH_MB2B)
    {
        return QString("%1 KB").arg(label);
    }
    else if( MH_MB2B <= size && size < MH_GB2B)
    {
        return QString("%1 MB").arg(label);
    }
    else if( MH_GB2B <= size && size < MH_TB2B)
    {
        return QString("%1 TB").arg(label);
    }
    else
    {
        return QString();
    }
}

QString MusicUtils::Number::speed2Label(qint64 size)
{
    return speed2LabelFromLabel(size, size2Number(size));
}

QString MusicUtils::Number::speed2LabelInt(qint64 size)
{
    return speed2LabelFromLabel(size, size2NumberInt(size));
}

qreal MusicUtils::Number::sizeByte2KByte(qint64 size)
{
    return size*1.0 / MH_KB;
}

qreal MusicUtils::Number::sizeByte2MByte(qint64 size)
{
    return sizeByte2KByte(size) / MH_MB;
}

qreal MusicUtils::Number::sizeByte2TByte(qint64 size)
{
    return sizeByte2MByte(size) / MH_GB;
}

QString MusicUtils::Number::speed2LabelFromLabel(qint64 size, const QString &label)
{
    if( size < MH_KB2B)
    {
        return QString("%1 B/s").arg(label);
    }
    else if( MH_KB2B <= size && size < MH_MB2B)
    {
        return QString("%1 KB/s").arg(label);
    }
    else if( MH_MB2B <= size && size < MH_GB2B)
    {
        return QString("%1 MB/s").arg(label);
    }
    else if( MH_GB2B <= size && size < MH_TB2B)
    {
        return QString("%1 TB/s").arg(label);
    }
    else
    {
        return QString();
    }
}
