#include "musicstringutils.h"

#include <QColor>

QString MusicUtils::String::removeStringBy(const QString &value, const QString &key)
{
    QString s = value;
    s.remove(key);
    if(s.contains(key))
    {
        s = removeStringBy(key);
    }
    return s;
}

QStringList MusicUtils::String::splitString(const QString &value)
{
    QStringList strings = value.split(" - ");
    if(strings.isEmpty() || strings.count() == 1)
    {
        strings = value.split("-");
    }
    return strings;
}

QString MusicUtils::String::artistName(const QString &value)
{
    QStringList s = splitString(value);
    if(s.count() >= 2)
    {
        int index = value.indexOf("-");
        return value.left(index).trimmed();
    }

    return value;
}

QString MusicUtils::String::songName(const QString &value)
{
    QStringList s = splitString(value);
    if(s.count() >= 2)
    {
        int index = value.indexOf("-") + 1;
        return value.right(value.length() - index).trimmed();
    }

    return value;
}

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
