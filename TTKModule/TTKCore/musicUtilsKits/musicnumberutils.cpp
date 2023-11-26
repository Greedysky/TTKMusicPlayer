#include "musicnumberutils.h"

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

int TTK::Number::bitrateToLevel(const QString &bitrate)
{
    if(bitrate.isEmpty())
    {
        return -1;
    }

    const QStringList &data(bitrate.split(" "));
    if(data.count() >= 2)
    {
        const int bit = data.front().trimmed().toInt();
        if(bit <= 0)
        {
            return -1;
        }
        else if(bit > 0 && bit <= TTK_BN_96)
        {
            return 0;
        }
        else if(bit > TTK_BN_96 && bit < TTK_BN_192)
        {
            return 1;
        }
        else if(bit >= TTK_BN_192 && bit <= TTK_BN_320)
        {
            return 2;
        }
        else
        {
            return 3;
        }
    }
    return -1;
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
        return TTK_BN_500;
    }
    else
    {
        return bitrate;
    }
}

void TTK::Number::bitrateToQuality(int level, QString &bitrate, QColor &color)
{
    bitrate = QObject::tr("UnKnow");
    color = QColor(131, 131, 131);

    switch(level)
    {
        case 0:
        {
            bitrate = QObject::tr("Low");
            color = QColor(211, 0, 70);
            break;
        }
        case 1:
        {
            bitrate = QObject::tr("Normal");
            color = QColor(0, 134, 211);
            break;
        }
        case 2:
        {
            bitrate = QObject::tr("High");
            color = QColor(236, 138, 48);
            break;
        }
        case 3:
        {
            bitrate = QObject::tr("LLess");
            color = QColor(117, 0, 206);
            break;
        }
        default: break;
    }
}
