#include "musicnumberutils.h"
#include "ttktime.h"

#include <QColor>
#include <QStringList>

static QString size2Number(qint64 size)
{
    if(size < TTK_SN_KB2B)
    {
        return QString::number(size * 1.0, 'f', 1);
    }
    else if(TTK_SN_KB2B <= size && size < TTK_SN_MB2B)
    {
        return QString::number(size * 1.0 / TTK_SN_KB2B, 'f', 1);
    }
    else if(TTK_SN_MB2B <= size && size < TTK_SN_GB2B)
    {
        return QString::number(size * 1.0 / TTK_SN_MB2B, 'f', 1);
    }
    else if(TTK_SN_GB2B <= size && size < TTK_SN_TB2B)
    {
        return QString::number(size * 1.0 / TTK_SN_GB2B, 'f', 1);
    }
    else
    {
        return QString::number(size * 1.0 / TTK_SN_TB2B, 'f', 1);
    }
}

QString TTK::Number::sizeByteToLabel(qint64 size)
{
    if(size < 0)
    {
        return "0.0B";
    }

    const QString &label = size2Number(size);
    if(size < TTK_SN_KB2B)
    {
        return QString("%1B").arg(label);
    }
    else if(TTK_SN_KB2B <= size && size < TTK_SN_MB2B)
    {
        return QString("%1K").arg(label);
    }
    else if(TTK_SN_MB2B <= size && size < TTK_SN_GB2B)
    {
        return QString("%1M").arg(label);
    }
    else if(TTK_SN_GB2B <= size && size < TTK_SN_TB2B)
    {
        return QString("%1G").arg(label);
    }
    else
    {
        return QString("%1T").arg(label);
    }
}

QString TTK::Number::sizeByteToLabel(const QString &duration, int bitrate)
{
    if(duration.isEmpty() || duration == TTK_DEFAULT_STR)
    {
        return "0.0B";
    }
    return TTK::Number::sizeByteToLabel(TTKTime::formatDuration(duration) * bitrate / 8.0);
}

QString TTK::Number::speedByteToLabel(qint64 size)
{
    if(size < 0)
    {
        return "0.0B/s";
    }

    const QString &label = size2Number(size);
    if(size < TTK_SN_KB2B)
    {
        return QString("%1B/s").arg(label);
    }
    else if(TTK_SN_KB2B <= size && size < TTK_SN_MB2B)
    {
        return QString("%1K/s").arg(label);
    }
    else if(TTK_SN_MB2B <= size && size < TTK_SN_GB2B)
    {
        return QString("%1M/s").arg(label);
    }
    else if(TTK_SN_GB2B <= size && size < TTK_SN_TB2B)
    {
        return QString("%1G/s").arg(label);
    }
    else
    {
        return QString("%1T/s").arg(label);
    }
}

int TTK::Number::bitrateToNormal(int bitrate)
{
    if(bitrate > TTK_BN_0 && bitrate <= TTK_BN_64)
    {
        return TTK_BN_32;
    }
    else if(bitrate > TTK_BN_64 && bitrate < TTK_BN_128)
    {
        return TTK_BN_128;
    }
    else if(bitrate > TTK_BN_128 && bitrate < TTK_BN_192)
    {
        return TTK_BN_192;
    }
    else if(bitrate > TTK_BN_192 && bitrate < TTK_BN_320)
    {
        return TTK_BN_320;
    }
    else if(bitrate > TTK_BN_320)
    {
        return TTK_BN_1000;
    }
    else
    {
        return bitrate;
    }
}

TTK::QueryQuality TTK::Number::bitrateToLevel(int bitrate)
{
    if(bitrate <= 0)
    {
        return QueryQuality::None;
    }
    else if(bitrate > 0 && bitrate <= TTK_BN_128)
    {
        return QueryQuality::Standard;
    }
    else if(bitrate > TTK_BN_128 && bitrate <= TTK_BN_192)
    {
        return QueryQuality::High;
    }
    else if(bitrate > TTK_BN_192 && bitrate <= TTK_BN_320)
    {
        return QueryQuality::Super;
    }
    else
    {
        return QueryQuality::Lossless;
    }
}

TTK::QueryQuality TTK::Number::bitrateToLevel(const QString &bitrate)
{
    if(bitrate.isEmpty())
    {
        return QueryQuality::None;
    }

    const QStringList &data(bitrate.split(TTK_SPACE));
    if(data.count() >= 2)
    {
        const int rate = data.front().trimmed().toInt();
        return TTK::Number::bitrateToLevel(rate);
    }
    return QueryQuality::None;
}

void TTK::Number::bitrateToQuality(TTK::QueryQuality level, QString &bitrate, QColor &color)
{
    bitrate = QObject::tr("UnKnow");
    color = QColor(131, 131, 131);

    switch(level)
    {
        case QueryQuality::Standard:
        {
            bitrate = QObject::tr("SD");
            color = QColor(211, 0, 70);
            break;
        }
        case QueryQuality::High:
        {
            bitrate = QObject::tr("HQ");
            color = QColor(0, 134, 211);
            break;
        }
        case QueryQuality::Super:
        {
            bitrate = QObject::tr("SQ");
            color = QColor(236, 138, 48);
            break;
        }
        case QueryQuality::Lossless:
        {
            bitrate = QObject::tr("CD");
            color = QColor(117, 0, 206);
            break;
        }
        default: break;
    }
}
