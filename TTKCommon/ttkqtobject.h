#ifndef TTKQTOBJECT_H
#define TTKQTOBJECT_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "ttkobject.h"
#include <QCoreApplication>

#define TTK_SERVICE_FULL        TTK::applicationPath() + TTK_SERVICE_SHL_NAME
#define TTK_APP_MAIN_FULL       TTK::applicationPath() + TTK_PDIR + TTK_APP_SHL_NAME

#ifdef Q_OS_UNIX
#  define TTK_INIT_FULL         TTK::applicationPath() + "TTKInit.sh"
#  define TTK_CONSOLE_FULL      TTK::applicationPath() + "TTKConsole.sh"
#  define TTK_ROUTINECOPY_FULL  TTK::applicationPath() + "TTKRoutineCopy.sh"
#  define TTK_ROUTINE_FULL      TTK::applicationPath() + TTK_PDIR + "TTKRoutine.sh"
#endif


/*! @brief The namespace of the application object.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    /*!
     * Get application work dir.
     */
    static QString applicationPath()
    {
        return QCoreApplication::applicationDirPath() + TTK_SEPARATOR;
    }
}

#endif // TTKQTOBJECT_H
