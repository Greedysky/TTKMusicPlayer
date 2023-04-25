#ifndef QKUGOUOBJECT_H
#define QKUGOUOBJECT_H

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

#include <QObject>

/*! @brief The namespace of the application ui object.
 * @author Greedysky <greedysky@163.com>
 */
namespace UI
{
    ///PushButton
    static const QString PushButtonStyle01 = " \
        QPushButton{ border:none; color:rgb(135, 135, 135); font-size:14px; } \
        QPushButton:hover{ color:rgb(104, 169, 236); } \
        QPushButton:checked{ color:rgb(40, 143, 231); }";

    static const QString PushButtonStyle02 = " \
        QPushButton{ color:rgb(104, 169, 236); }";

    ///Widget
    static const QString WidgetStyle01 = " \
        QWidget{ background:white; }";

}

#endif // QKUGOUOBJECT_H
