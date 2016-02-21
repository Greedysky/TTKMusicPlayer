#ifndef MUSICUTILS_H
#define MUSICUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

class QComboBox;

#include "musicobject.h"
#include "musicglobaldefine.h"

class MUSIC_CORE_EXPORT MusicUtils
{
public:
    MusicUtils(){}

    static void dirIsExist(const QString& name);
    static void checkTheDirectoryExist();
    static bool checkTheFileExist();

    static QString getLanguageName(int index);

    static void setLabelFont(QWidget *widget, int size);
    static void setComboboxText(QComboBox *object, const QString &text);

    static QString fileSize2Label(qint64 size);
    static QString fileSize2Normal(qint64 size);
    static QString fileSpeed2Normal(qint64 size);

    static qreal fileSizeByte2KByte(qint64 size);
    static qreal fileSizeByte2MByte(qint64 size);

    static quint64 dirSize(const QString &dirName);
    static void checkCacheSize(quint64 cacheSize, bool disabled, const QString &path);

};


#endif // MUSICUTILS

