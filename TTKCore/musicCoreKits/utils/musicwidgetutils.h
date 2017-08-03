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

/*! @brief The class of the utils widget object namespace.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUtils
{
    namespace Widget
    {
        MUSIC_SUPER_EXPORT void setLabelFontSize(QWidget *widget, int size);
        /*!
         * Set widget label font.
         */
        MUSIC_SUPER_EXPORT void setLabelFontStyle(QWidget *widget, MusicObject::FontStyleMode type);
        /*!
         * Set widget label font.
         */
        MUSIC_SUPER_EXPORT QString elidedText(const QFont &font, const QString &text,
                                             Qt::TextElideMode mode, int width);
        /*!
         * Set text elided text by font.
         */
        MUSIC_SUPER_EXPORT void setTransparent(QWidget *widget, int alpha);
        /*!
         * Set widget transparent.
         */
        MUSIC_SUPER_EXPORT void setComboboxText(QComboBox *object, const QString &text);
        /*!
         * Set Combobox text.
         */
        MUSIC_SUPER_EXPORT void widgetToRound(QWidget *w, int ratioX, int ratioY);
        /*!
         * Set widget to round by ratioX and ratioY.
         */
        MUSIC_SUPER_EXPORT void fusionPixmap(QPixmap &bg, const QPixmap &fg, const QPoint &pt);
        /*!
         * Set fusion two image.
         */
        MUSIC_SUPER_EXPORT QPixmap pixmapToRound(const QPixmap &src, const QSize &size, int ratioX, int ratioY);
        /*!
         * Set pixmap to round by ratio.
         */
        MUSIC_SUPER_EXPORT QPixmap pixmapToRound(const QPixmap &src, const QRect &rect, int ratioX, int ratioY);
        /*!
         * Set pixmap to round by ratio.
         */
        MUSIC_SUPER_EXPORT QPixmap pixmapToRound(const QPixmap &src, const QPixmap &mask, const QSize &size);
        /*!
         * Set pixmap to round by ratio.
         */
        MUSIC_SUPER_EXPORT QBitmap getBitmapMask(const QRect &rect, int ratioX, int ratioY);
        /*!
         * Get bitmap mask from rect.
         */

        template<class T>
        MUSIC_SUPER_EXPORT T reRenderValue(T key, T alpha, T value)
        {
            if(alpha < 0) return 0;
            else if(alpha > key) return key;

            return (key - alpha)*1.0/100*value + alpha;
        }
        /*!
         * Rerender the custum value.
         */
        MUSIC_SUPER_EXPORT int reRenderAlpha(int alpha, int value);
        /*!
         * Rerender the image alpha.
         */
        MUSIC_SUPER_EXPORT void reRenderImage(int delta, const QImage *input, QImage *output);
        /*!
         * Rerender the image by color burn transform.
         */
        MUSIC_SUPER_EXPORT uint colorBurnTransform(int c, int delta);
        /*!
         * Image color burn transform.
         */
    }
}

#endif // MUSICWIDGETUTILS_H
