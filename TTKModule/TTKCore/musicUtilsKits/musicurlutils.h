#ifndef MUSICURLUTILS_H
#define MUSICURLUTILS_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include "musicglobaldefine.h"

/*! @brief The namespace of the utils url.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    namespace Url
    {
        /*!
         * Execute file from local by path or other url.
         */
        TTK_MODULE_EXPORT bool execute(const QString &path);

        /*!
         * Open file from local by path or other url.
         */
        TTK_MODULE_EXPORT bool openUrl(const QString &path, bool local = true);

        /*!
         * Encode the data into url.
         */
        TTK_MODULE_EXPORT QString urlEncode(QString &data);
        /*!
         * Decode the data into url.
         */
        TTK_MODULE_EXPORT QString urlDecode(QString &data);
        /*!
         * Encode the data into url.
         */
        TTK_MODULE_EXPORT QByteArray urlEncode(QByteArray &data);
        /*!
         * Decode the data into url.
         */
        TTK_MODULE_EXPORT QByteArray urlDecode(QByteArray &data);

        /*!
         * Pretty encode the data into url.
         */
        TTK_MODULE_EXPORT QString urlPrettyEncode(QString &data);
        /*!
         * Pretty decode the data into url.
         */
        TTK_MODULE_EXPORT QString urlPrettyDecode(QString &data);
        /*!
         * Pretty encode the data into url.
         */
        TTK_MODULE_EXPORT QByteArray urlPrettyEncode(QByteArray &data);
        /*!
         * Pretty decode the data into url.
         */
        TTK_MODULE_EXPORT QByteArray urlPrettyDecode(QByteArray &data);

    }
}

#endif // MUSICURLUTILS_H
