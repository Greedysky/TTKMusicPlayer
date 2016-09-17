#include "pinyinresource.h"

#include <QFile>
#include <QTextStream>

QMap<QString, QString> PinyinResource::getPinyinResource()
{
    return getResource(":/data/pinyin");
}

QMap<QString, QString> PinyinResource::getMutilPinyinResource()
{
    return getResource(":/data/mutil_pinyin");
}

QMap<QString, QString> PinyinResource::getChineseResource()
{
    return getResource(":/data/single_pinyin");
}

QMap<QString, QString> PinyinResource::getResource(const QString &resourceName)
{
    QMap<QString, QString> map;
    QFile file(resourceName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return map;
    }

    QTextStream in(&file);
    in.setCodec("utf-8");

    QString line;
    while((line = in.readLine()) != QString())
    {
        QStringList tokens = line.trimmed().split("=");
        map.insert(tokens[0], tokens[1]);
    }

    file.close();
    return map;
}
