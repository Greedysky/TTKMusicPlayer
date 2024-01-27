#ifndef MUSICWIDGETUTILS_H
#define MUSICWIDGETUTILS_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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
 ***************************************************************************/

#include <QPainter>
#include "musicuiobject.h"
#include "musicglobaldefine.h"

class QMenu;
class QComboBox;

/*! @brief The namespace of the utils widget.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    namespace Widget
    {
        /*!
         * Set widget font.
         */
        TTK_MODULE_EXPORT void setFontSize(QWidget *widget, int size);
        /*!
         * Set widget font style.
         */
        TTK_MODULE_EXPORT void setFontStyle(QWidget *widget, TTK::FontStyleMode type);

        /*!
         * Set text elided text by font.
         */
        TTK_MODULE_EXPORT QString elidedText(const QFont &font, const QString &text, Qt::TextElideMode mode, int width);

        /*!
         * Get text width by font.
         */
        TTK_MODULE_EXPORT int fontTextWidth(const QFont &font, const QString &text);
        /*!
         * Get font height by font.
         */
        TTK_MODULE_EXPORT int fontTextHeight(const QFont &font);

        /*!
         * Adjust the menu position.
         */
        TTK_MODULE_EXPORT void adjustMenuPosition(QMenu *menu);
        /*!
         * Adjust the widget in screen center.
         */
        TTK_MODULE_EXPORT void adjustWidgetPosition(QWidget *widget);

        /*!
         * Set widget transparent.
         */
        TTK_MODULE_EXPORT void setTransparent(QWidget *widget, int alpha);
        /*!
         * Set widget transparent.
         */
        TTK_MODULE_EXPORT void setTransparent(QWidget *widget, const QColor &color);

        /*!
         * Set ComboBox text.
         */
        TTK_MODULE_EXPORT void setComboBoxText(QComboBox *widget, const QString &text);
        /*!
         * Set widget to round by ratioX and ratioY.
         */
        TTK_MODULE_EXPORT void widgetToRound(QWidget *widget, int ratioX, int ratioY);

        /*!
         * Generate vertical scrollArea format style.
         */
        TTK_MODULE_EXPORT void generateVScrollAreaFormat(QWidget *widget, QWidget *parent, bool background = true);
        /*!
         * Generate horizontal scrollArea format style.
         */
        TTK_MODULE_EXPORT void generateHScrollAreaFormat(QWidget *widget, QWidget *parent, bool background = true);
        /*!
         * Generate combobox format style.
         */
        TTK_MODULE_EXPORT void generateComboBoxFormat(QComboBox *widget, const QString &style = {});

    }
}

#endif // MUSICWIDGETUTILS_H
