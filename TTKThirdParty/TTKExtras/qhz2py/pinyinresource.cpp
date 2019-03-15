#include "pinyinresource.h"

#include <QFile>
#include <QTextStream>

MStringMap PinyinResource::getPinyinResource()
{
    return getResource(":/data/pinyin");
}

MStringMap PinyinResource::getMutilPinyinResource()
{
    return getResource(":/data/mutil_pinyin");
}

MStringMap PinyinResource::getChineseResource()
{
    return getResource(":/data/single_pinyin");
}

MStringMap PinyinResource::getResource(const QString &resourceName)
{
    MStringMap map;
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
        const QStringList &tokens = line.trimmed().split("=");
        if(tokens.count() >= 2)
        {
            map.insert(tokens[0], tokens[1]);
        }
    }

    file.close();
    return map;
}
