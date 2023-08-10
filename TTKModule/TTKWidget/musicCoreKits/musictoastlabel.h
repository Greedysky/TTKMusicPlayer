#ifndef MUSICTOASTLABEL_H
#define MUSICTOASTLABEL_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include "ttktoastlabel.h"

/*! @brief The class of the toast widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicToastLabel : public TTKToastLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicToastLabel)
public:
    /*!
     * Object constructor.
     */
    explicit MusicToastLabel(QWidget *parent = nullptr);
    explicit MusicToastLabel(const QString &text, QWidget *parent = nullptr);

    /*!
     * Popup the toast widget in parent widget.
     */
    static void popup(const QString &text);
    using TTKToastLabel::popup;

};

#endif // MUSICTOASTLABEL_H
