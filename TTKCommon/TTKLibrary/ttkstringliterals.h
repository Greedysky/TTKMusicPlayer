#ifndef TTKSTRINGLITERALS_H
#define TTKSTRINGLITERALS_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#include "ttkmoduleexport.h"
#if TTK_QT_VERSION_CHECK(5,10,0)
#  include <QStringView>
#endif

/*! @brief The namespace of the string literals.
 * @author Greedysky <greedysky@163.com>
 */
namespace Qt
{
    namespace Literals
    {
        namespace StringLiterals
        {
#if !TTK_QT_VERSION_CHECK(6,4,0)
            inline QString operator""_s(const char *str, size_t size) noexcept
            {
                return QString::fromUtf8(str, static_cast<int>(size));
            }

            inline QString operator""_s(const wchar_t *str, size_t size) noexcept
            {
                return QString::fromWCharArray(str, static_cast<int>(size));
            }

            inline QString operator""_s(const char16_t *str, size_t size) noexcept
            {
                return QString::fromUtf16(str, static_cast<int>(size));
            }

            inline QString operator""_s(const char32_t *str, size_t size) noexcept
            {
                return QString::fromUcs4(str, static_cast<int>(size));
            }

            inline QByteArray operator""_ba(const char *str, size_t size) noexcept
            {
                return operator""_s(str, size).toUtf8();
            }

            inline QByteArray operator""_ba(const wchar_t *str, size_t size) noexcept
            {
                return operator""_s(str, size).toUtf8();
            }

            inline QByteArray operator""_ba(const char16_t *str, size_t size) noexcept
            {
                return operator""_s(str, size).toUtf8();
            }

            inline QByteArray operator""_ba(const char32_t *str, size_t size) noexcept
            {
                return operator""_s(str, size).toUtf8();
            }
#endif

#if TTK_QT_VERSION_CHECK(5,10,0) && !TTK_QT_VERSION_CHECK(6,10,0)
            inline QStringView operator""_sv(const char16_t *str, size_t size) noexcept
            {
                return QStringView(str, size);
            }
#endif
        }
    }
}

#endif // TTKSTRINGLITERALS_H
