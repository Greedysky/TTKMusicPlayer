#include "musicstringutils.h"

#include <QColor>

QList<QColor> MusicUtils::String::readColorConfig(const QString &value)
{
    QList<QColor> colors;
    QStringList rgbs = value.split(';', QString::SkipEmptyParts);
    foreach(const QString &rgb, rgbs)
    {
        QStringList var = rgb.split(',');
        if(var.count() != 3)
        {
            continue;
        }
        colors << QColor(var[0].toInt(), var[1].toInt(), var[2].toInt());
    }
    return colors;
}

QString MusicUtils::String::writeColorConfig(const QColor &color)
{
    QString value;
    value.append(QString("%1,%2,%3").arg(color.red()).arg(color.green()).arg(color.blue()));
    return value;
}

QString MusicUtils::String::writeColorConfig(const QList<QColor> &colors)
{
    QString value;
    foreach(const QColor &rgb, colors)
    {
        value.append(QString("%1,%2,%3").arg(rgb.red()).arg(rgb.green()).arg(rgb.blue()));
        value.append(";");
    }
    return value;
}
