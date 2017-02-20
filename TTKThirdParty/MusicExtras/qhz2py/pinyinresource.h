#ifndef PINYINRESOURCE_H
#define PINYINRESOURCE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMap>
#include <QStringList>
#include "musicprivate.h"
#include "musicextrasglobaldefine.h"

/*! @brief The class of the pinyin resource.
 * @author jiangXiaoBai <2499971906@qq.com>
 */
class MUSIC_EXTRAS_EXPORT PinyinResource
{
public:
    static QMap<QString, QString> getPinyinResource();
    /*!
     * Get pinyin resource.
     */
    static QMap<QString, QString> getMutilPinyinResource();
    /*!
     * Get mutil pinyin resource.
     */
    static QMap<QString, QString> getChineseResource();
    /*!
     * Get chinese resource.
     */

private:
    static QMap<QString, QString> getResource(const QString &resourceName);

};

#endif // PINYINRESOURCE_H
