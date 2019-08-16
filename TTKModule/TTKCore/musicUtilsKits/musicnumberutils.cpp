#include "musicnumberutils.h"
#include "musicnumberdefine.h"

#include <QColor>
#include <QStringList>

QString MusicUtils::Number::size2Number(qint64 size)
{
    if(size < MH_KB2B)
    {
        return QString::number(size*1.0, 'f', 2);
    }
    else if(MH_KB2B <= size && size < MH_MB2B)
    {
        return QString::number(size*1.0/MH_KB2B, 'f', 2);
    }
    else if(MH_MB2B <= size && size < MH_GB2B)
    {
        return QString::number(size*1.0/MH_MB2B, 'f', 2);
    }
    else if(MH_GB2B <= size && size < MH_TB2B)
    {
        return QString::number(size*1.0/MH_GB2B, 'f', 2);
    }
    else
    {
        return QString::number(size*1.0/MH_TB2B, 'f', 2);
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
    if(size < 0)
    {
        return "0.00B";
    }

    const QString &label = size2Number(size);
    if(size < MH_KB2B)
    {
        return QString("%1B").arg(label);
    }
    else if(MH_KB2B <= size && size < MH_MB2B)
    {
        return QString("%1K").arg(label);
    }
    else if(MH_MB2B <= size && size < MH_GB2B)
    {
        return QString("%1M").arg(label);
    }
    else if(MH_GB2B <= size && size < MH_TB2B)
    {
        return QString("%1G").arg(label);
    }
    else
    {
        return QString("%1T").arg(label);
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
    if(size < MH_KB2B)
    {
        return QString("%1B/s").arg(label);
    }
    else if(MH_KB2B <= size && size < MH_MB2B)
    {
        return QString("%1K/s").arg(label);
    }
    else if(MH_MB2B <= size && size < MH_GB2B)
    {
        return QString("%1M/s").arg(label);
    }
    else if(MH_GB2B <= size && size < MH_TB2B)
    {
        return QString("%1G/s").arg(label);
    }
    else
    {
        return QString("%1T/s").arg(label);
    }
}

void MusicUtils::Number::transfromBitrateToQuality(int level, QString &bitString, QColor &color)
{
    bitString = QObject::tr("UnKnow");
    color = QColor(131, 131, 131);
    switch(level)
    {
        case 0:
            bitString = QObject::tr("Low");
            color = QColor(211, 0, 70);
            break;
        case 1:
            bitString = QObject::tr("Normal");
            color = QColor(0, 134, 211);
            break;
        case 2:
            bitString = QObject::tr("High");
            color = QColor(236, 138, 48);
            break;
        case 3:
            bitString = QObject::tr("LLess");
            color = QColor(117, 0, 206);
            break;
        default: break;
    }
}

void MusicUtils::Number::transfromBitrateToQuality(int level, QString &bitString)
{
    QColor color;
    transfromBitrateToQuality(level, bitString, color);
}

QString MusicUtils::Number::transfromBitrateToQuality(int level)
{
    QColor color;
    QString bitString;
    transfromBitrateToQuality(level, bitString, color);
    return bitString;
}

int MusicUtils::Number::transfromBitrateToLevel(const QString &bitrate)
{
    if(bitrate.isEmpty())
    {
        return -1;
    }

    const QStringList &data(bitrate.split(" "));
    if(data.count() >= 2)
    {
        int bit = data.front().trimmed().toInt();
        if(bit <= 0)
            return -1;
        else if(bit > 0 && bit <= MB_96)
            return 0;
        else if(bit > MB_96 && bit < MB_192)
            return 1;
        else if(bit >= MB_192 && bit <= MB_320)
            return 2;
        else
            return 3;
    }
    return -1;
}

int MusicUtils::Number::transfromBitrateToNormal(int bitrate)
{
    if(bitrate > MB_0 && bitrate <= MB_64)
        return MB_32;
    else if(bitrate > MB_64 && bitrate < MB_128)
        return MB_128;
    else if(bitrate > MB_128 && bitrate < MB_192)
        return MB_192;
    else if(bitrate > MB_192 && bitrate < MB_320)
        return MB_320;
    else if(bitrate > MB_320)
        return MB_500;
    else
        return bitrate;
}

int MusicUtils::Number::transfromBitrateToNormal(const QString &bitrate)
{
    if(bitrate == "e")
        return MB_32;
    else if(bitrate == "f")
        return MB_128;
    else if(bitrate == "l")
        return MB_192;
    else if(bitrate == "h")
        return MB_320;
    else if(bitrate == "s")
        return MB_500;
    else
        return MB_128;
}
