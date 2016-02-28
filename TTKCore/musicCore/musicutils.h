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

/*! @brief The namespace of the utils object.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicUtils
{
public:
    MusicUtils(){}
    /*!
     * Object contsructor.
     */

    static void dirIsExist(const QString& name);
    /*!
     * Check current dir is exist, no, just create it.
     */
    static void checkTheDirectoryExist();
    /*!
     * Check the related dir is exist.
     */
    static bool checkTheFileExist();
    /*!
     * Check the related file is exist.
     */

    static QString getLanguageName(int index);
    /*!
     * Get Language Name by given index.
     */

    static void setLabelFont(QWidget *widget, int size);
    /*!
     * Set widget label font.
     */
    static void setTransparent(QWidget *widget, int alpha);
    /*!
     * Set widget transparent.
     */
    static void setComboboxText(QComboBox *object, const QString &text);
    /*!
     * Set Combobox text.
     */

    static QString size2Number(qint64 size);
    /*!
     * Transform value to cp float value.
     */
    static QString size2NumberInt(qint64 size);
    /*!
     * Transform value to cp int value.
     */
    static QString size2Label(qint64 size);
    /*!
     * Transform value to cp value.
     */
    static QString speed2Label(qint64 size);
    /*!
     * Transform value to speed float value.
     */
    static QString speed2LabelInt(qint64 size);
    /*!
     * Transform value to speed int value.
     */

    static qreal sizeByte2KByte(qint64 size);
    /*!
     * Transform value to kb entity.
     */
    static qreal sizeByte2MByte(qint64 size);
    /*!
     * Transform value to mb entity.
     */
    static qreal sizeByte2TByte(qint64 size);
    /*!
     * Transform value to tb entity.
     */

    static quint64 dirSize(const QString &dirName);
    /*!
     * Get given dir size.
     */
    static void checkCacheSize(quint64 cacheSize, bool disabled, const QString &path);
    /*!
     * Check given dir size is bigger than given size.
     */

private:
    static QString speed2LabelFromLabel(qint64 size, const QString &label);
    /*!
     * Transform value to speed value base.
     */

};


#endif // MUSICUTILS

