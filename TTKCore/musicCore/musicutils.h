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
#include "musicnumberdefine.h"

/*! @brief The class of the utils object namespace.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUtils
{
    namespace UCore
    {
        MUSIC_CORE_EXPORT void dirIsExist(const QString& name);
        /*!
         * Check current dir is exist, no, just create it.
         */
        MUSIC_CORE_EXPORT void checkTheDirectoryExist();
        /*!
         * Check the related dir is exist.
         */
        MUSIC_CORE_EXPORT bool checkTheFileExist();
        /*!
         * Check the related file is exist.
         */
        MUSIC_CORE_EXPORT quint64 dirSize(const QString &dirName);
        /*!
         * Get given dir size.
         */
        MUSIC_CORE_EXPORT void checkCacheSize(quint64 cacheSize, bool disabled, const QString &path);
        /*!
         * Check given dir size is bigger than given size.
         */
        MUSIC_CORE_EXPORT QFileInfoList findFile(const QString &path, const QStringList &filter);
        /*!
         * To find file by dir path.
         */

        MUSIC_CORE_EXPORT QString getLanguageName(int index);
        /*!
         * Get Language Name by given index.
         */

        MUSIC_CORE_EXPORT bool openUrl(const QString &path, bool local = true);
        /*!
         * Open file from local by path or net url.
         */

        MUSIC_CORE_EXPORT QString toUnicode(const char *chars, const char *format = "GBK");
        /*!
         * Set string to unicode string by format.
         */
        MUSIC_CORE_EXPORT QString toUnicode(const QByteArray &chars, const char *format = "GBK");
        /*!
         * Set string to unicode string by format.
         */
        MUSIC_CORE_EXPORT QByteArray fromUnicode(const QString &chars, const char *format = "GBK");
        /*!
         * Set string from unicode string by format.
         */
        MUSIC_CORE_EXPORT void setLocalCodec(const char *format = "utf-8");
        /*!
         * Set local codec by format.
         */
        MUSIC_CORE_EXPORT const char *toLocal8Bit(const QString &str);
        /*!
         * Trasform string to local 8bit char.
         */
        MUSIC_CORE_EXPORT const char *toUtf8(const QString &str);
        /*!
         * Trasform string to utf8 char.
         */

    }

    namespace UWidget
    {
        MUSIC_CORE_EXPORT void setLabelFontSize(QWidget *widget, int size);
        /*!
         * Set widget label font.
         */
        MUSIC_CORE_EXPORT void setLabelFontStyle(QWidget *widget, MusicObject::FontStyleType type);
        /*!
         * Set widget label font.
         */
        MUSIC_CORE_EXPORT QString elidedText(const QFont &font, const QString &text,
                                             Qt::TextElideMode mode, int width);
        /*!
         * Set text elided text by font.
         */
        MUSIC_CORE_EXPORT void setTransparent(QWidget *widget, int alpha);
        /*!
         * Set widget transparent.
         */
        MUSIC_CORE_EXPORT void setComboboxText(QComboBox *object, const QString &text);
        /*!
         * Set Combobox text.
         */
        MUSIC_CORE_EXPORT void widgetToRound(QWidget *w, int ratioX, int ratioY);
        /*!
         * Set widget to round by ratioX and ratioY.
         */
        MUSIC_CORE_EXPORT QPixmap pixmapToRound(const QPixmap &src, const QSize &size, int ratioX, int ratioY);
        /*!
         * Set pixmap to round by ratio.
         */
        MUSIC_CORE_EXPORT QPixmap pixmapToRound(const QPixmap &src, const QRect &rect, int ratioX, int ratioY);
        /*!
         * Set pixmap to round by ratio.
         */
        MUSIC_CORE_EXPORT QBitmap getBitmapMask(const QRect &rect, int ratioX, int ratioY);
        /*!
         * Get bitmap mask from rect.
         */

    }

    namespace UNumber
    {
        MUSIC_CORE_EXPORT QString size2Number(qint64 size);
        /*!
         * Transform value to cp float value.
         */
        MUSIC_CORE_EXPORT QString size2NumberInt(qint64 size);
        /*!
         * Transform value to cp int value.
         */
        MUSIC_CORE_EXPORT QString size2Label(qint64 size);
        /*!
         * Transform value to cp value.
         */
        MUSIC_CORE_EXPORT QString speed2Label(qint64 size);
        /*!
         * Transform value to speed float value.
         */
        MUSIC_CORE_EXPORT QString speed2LabelInt(qint64 size);
        /*!
         * Transform value to speed int value.
         */

        MUSIC_CORE_EXPORT qreal sizeByte2KByte(qint64 size);
        /*!
         * Transform value to kb entity.
         */
        MUSIC_CORE_EXPORT qreal sizeByte2MByte(qint64 size);
        /*!
         * Transform value to mb entity.
         */
        MUSIC_CORE_EXPORT qreal sizeByte2TByte(qint64 size);
        /*!
         * Transform value to tb entity.
         */

        MUSIC_CORE_EXPORT QString speed2LabelFromLabel(qint64 size, const QString &label);
        /*!
         * Transform value to speed value base.
         */

    }
}


#endif // MUSICUTILS

