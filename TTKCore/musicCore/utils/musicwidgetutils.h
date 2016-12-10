#ifndef MUSICWIDGETUTILS_H
#define MUSICWIDGETUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

class QComboBox;

#include "musicobject.h"
#include "musicnumberdefine.h"
#include "musicglobaldefine.h"

/*! @brief The class of the utils object namespace.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUtils
{
    namespace Widget
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
        MUSIC_CORE_EXPORT void fusionPixmap(QPixmap &bg, const QPixmap &fg, const QPoint &pt);
        /*!
         * Set fusion two image.
         */
        MUSIC_CORE_EXPORT QPixmap pixmapToRound(const QPixmap &src, const QSize &size, int ratioX, int ratioY);
        /*!
         * Set pixmap to round by ratio.
         */
        MUSIC_CORE_EXPORT QPixmap pixmapToRound(const QPixmap &src, const QRect &rect, int ratioX, int ratioY);
        /*!
         * Set pixmap to round by ratio.
         */
        MUSIC_CORE_EXPORT QPixmap pixmapToRound(const QPixmap &src, const QPixmap &mask, const QSize &size);
        /*!
         * Set pixmap to round by ratio.
         */
        MUSIC_CORE_EXPORT QBitmap getBitmapMask(const QRect &rect, int ratioX, int ratioY);
        /*!
         * Get bitmap mask from rect.
         */
    }
}

#endif // MUSICWIDGETUTILS_H
