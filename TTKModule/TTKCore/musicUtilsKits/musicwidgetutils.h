#ifndef MUSICWIDGETUTILS_H
#define MUSICWIDGETUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

class QComboBox;

#include "musicobject.h"
#include "musicnumberdefine.h"
#include "musicglobaldefine.h"

/*! @brief The namespace of the utils widget.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUtils
{
    namespace Widget
    {
        /*!
         * Set widget label font.
         */
        MUSIC_UTILS_EXPORT void setLabelFontSize(QWidget *widget, int size);
        /*!
         * Set widget label font.
         */
        MUSIC_UTILS_EXPORT void setLabelFontStyle(QWidget *widget, MusicObject::FontStyleMode type);
        /*!
         * Set text elided text by font.
         */
        MUSIC_UTILS_EXPORT QString elidedText(const QFont &font, const QString &text, Qt::TextElideMode mode, int width);
        /*!
         * Set widget transparent.
         */
        MUSIC_UTILS_EXPORT void setTransparent(QWidget *widget, int alpha);
        /*!
         * Set ComboBox text.
         */
        MUSIC_UTILS_EXPORT void setComboBoxText(QComboBox *object, const QString &text);
        /*!
         * Set widget to round by ratioX and ratioY.
         */
        MUSIC_UTILS_EXPORT void widgetToRound(QWidget *w, int ratioX, int ratioY);
        /*!
         * Set fusion two image.
         */
        MUSIC_UTILS_EXPORT void fusionPixmap(QPixmap &back, const QPixmap &front, const QPoint &pt);
        /*!
         * Set pixmap to round by ratio.
         */
        MUSIC_UTILS_EXPORT QPixmap pixmapToRound(const QPixmap &src, const QSize &size, int ratioX, int ratioY);
        /*!
         * Set pixmap to round by ratio.
         */
        MUSIC_UTILS_EXPORT QPixmap pixmapToRound(const QPixmap &src, const QRect &rect, int ratioX, int ratioY);
        /*!
         * Set pixmap to round by ratio.
         */
        MUSIC_UTILS_EXPORT QPixmap pixmapToRound(const QPixmap &src, const QPixmap &mask, const QSize &size);
        /*!
         * Get bitmap mask from rect.
         */
        MUSIC_UTILS_EXPORT QBitmap getBitmapMask(const QRect &rect, int ratioX, int ratioY);
        /*!
         * Get pximap data.
         */
        MUSIC_UTILS_EXPORT QByteArray getPixmapData(const QPixmap &pix);

        /*!
         * Rerender the custum value.
         */
        template <typename T>
        MUSIC_UTILS_EXPORT T reRenderValue(const T &key, const T &alpha, const T &value)
        {
            if(alpha < 0) return 0;
            else if(alpha > key) return key;

            return (key - alpha)*1.0/100*value + alpha;
        }

        /*!
         * Rerender the image alpha.
         */
        MUSIC_UTILS_EXPORT int reRenderAlpha(int alpha, int value);
        /*!
         * Rerender the image by color burn transform.
         */
        MUSIC_UTILS_EXPORT void reRenderImage(int delta, const QImage *input, QImage *output);
        /*!
         * Image color burn transform.
         */
        MUSIC_UTILS_EXPORT int colorBurnTransform(int c, int delta);

    }
}

#endif // MUSICWIDGETUTILS_H
