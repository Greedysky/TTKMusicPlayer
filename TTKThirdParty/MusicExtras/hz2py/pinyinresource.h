#ifndef PINYINRESOURCE_H
#define PINYINRESOURCE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMap>
#include <QStringList>
#include "musicprivate.h"
#include "musicextrasglobaldefine.h"

/**
 * Pinyin resource class
 */
class MUSIC_EXTRAS_EXPORT PinyinResource
{
public:
    static QString getClassName();
    static QMap<QString, QString> getPinyinResource();
    static QMap<QString, QString> getMutilPinyinResource();
    static QMap<QString, QString> getChineseResource();

private:
    PinyinResource() {}

    static QMap<QString, QString> getResource(const QString &resourceName);

};

#endif // PINYINRESOURCE_H
