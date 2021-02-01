/* =================================================
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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

#ifndef CHARCHECKER_H
#define CHARCHECKER_H

#include <QMap>
#include <QString>
#include "qmmp_export.h"

class CheckerBase;

/*! @brief The class of the char check.
 * @author Greedysky <greedysky@163.com>
 */
class QMMP_EXPORT CharChecker
{
public:
    enum Type
    {
        Unknown = -1,
        ASCII,
        BIG5,
        GB18030,
        GBK,
        SHIFT_JIS,
        UTF8
    };

    CharChecker();
    ~CharChecker();

    Type check(const QString &str);

private:
    QMap<Type, CheckerBase*> m_checkers;

};

#endif // CHARCHECKER_H
