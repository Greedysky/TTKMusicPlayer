#ifndef TTKSUPERENUM_H
#define TTKSUPERENUM_H

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

#include <unordered_map>
#include "ttkmoduleexport.h"

/*! @brief The class of the super enum.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKSuperEnum
{
public:
    static constexpr int Null = -1;

public:
    /*!
     * Object constructor.
     */
    TTKSuperEnum(const QString &value);

    /*!
     * Map key int to string.
     */
    QString keyToString(int value) const;
    /*!
     * Map key string to int.
     */
    int stringToKey(const QString &name) const;

private:
    std::unordered_map<int, QString> m_enumValue;

};

#define TTK_SUPER_ENUM(Type, ...) \
  static const TTKSuperEnum Type ## EnumObject(#__VA_ARGS__); \
  class TTK_MODULE_EXPORT Type { \
  public: \
    enum { kNull = TTKSuperEnum::Null, __VA_ARGS__ }; \
  public: \
    Type() : m_data(Type::kNull) {} \
    Type(const Type &v) : m_data(v.m_data) {} \
    Type(int v) : m_data(v) {} \
    Type(const QString &name) : m_data(Type ## EnumObject.stringToKey(name)) {} \
    QString toString() const { return Type ## EnumObject.keyToString(m_data); } \
    int toInt() const { return m_data; } \
    operator int () const { return m_data; } \
    operator QString () const { return toString(); } \
    Type & operator =(int v) { m_data = v; return *this; } \
    Type & operator =(const QString &v) { m_data = Type ## EnumObject.stringToKey(v); return *this; } \
  private: \
    int m_data; \
  }

#define TTK_ENUM_TYPE(Enum) std::underlying_type<Enum>::type
#define TTK_DECLARE_OPERATORS_FOR_ENUM(Enum) \
inline constexpr Enum operator~(const Enum lhs) { return TTKStaticCast(Enum, ~TTKStaticCast(TTK_ENUM_TYPE(Enum), lhs)); } \
inline constexpr Enum operator!(const Enum lhs) { return TTKStaticCast(Enum, !TTKStaticCast(TTK_ENUM_TYPE(Enum), lhs)); } \
inline constexpr bool operator>(const Enum lhs, const Enum rhs) { return TTKStaticCast(TTK_ENUM_TYPE(Enum), lhs) > TTKStaticCast(TTK_ENUM_TYPE(Enum), rhs); } \
inline constexpr bool operator<(const Enum lhs, const Enum rhs) { return TTKStaticCast(TTK_ENUM_TYPE(Enum), lhs) < TTKStaticCast(TTK_ENUM_TYPE(Enum), rhs); } \
inline constexpr bool operator>=(const Enum lhs, const Enum rhs) { return TTKStaticCast(TTK_ENUM_TYPE(Enum), lhs) >= TTKStaticCast(TTK_ENUM_TYPE(Enum), rhs); } \
inline constexpr bool operator<=(const Enum lhs, const Enum rhs) { return TTKStaticCast(TTK_ENUM_TYPE(Enum), lhs) <= TTKStaticCast(TTK_ENUM_TYPE(Enum), rhs); } \
inline constexpr bool operator==(const Enum lhs, const Enum rhs) { return TTKStaticCast(TTK_ENUM_TYPE(Enum), lhs) == TTKStaticCast(TTK_ENUM_TYPE(Enum), rhs); } \
inline constexpr bool operator!=(const Enum lhs, const Enum rhs) { return TTKStaticCast(TTK_ENUM_TYPE(Enum), lhs) != TTKStaticCast(TTK_ENUM_TYPE(Enum), rhs); } \
inline constexpr Enum operator&(const Enum lhs, const Enum rhs) { return TTKStaticCast(Enum, TTKStaticCast(TTK_ENUM_TYPE(Enum), lhs) & TTKStaticCast(TTK_ENUM_TYPE(Enum), rhs)); } \
inline constexpr Enum operator|(const Enum lhs, const Enum rhs) { return TTKStaticCast(Enum, TTKStaticCast(TTK_ENUM_TYPE(Enum), lhs) | TTKStaticCast(TTK_ENUM_TYPE(Enum), rhs)); } \
inline constexpr Enum operator^(const Enum lhs, const Enum rhs) { return TTKStaticCast(Enum, TTKStaticCast(TTK_ENUM_TYPE(Enum), lhs) ^ TTKStaticCast(TTK_ENUM_TYPE(Enum), rhs)); } \
inline constexpr bool operator||(const Enum lhs, const Enum rhs) { return TTKStaticCast(TTK_ENUM_TYPE(Enum), lhs) || TTKStaticCast(TTK_ENUM_TYPE(Enum), rhs); } \
inline constexpr bool operator&&(const Enum lhs, const Enum rhs) { return TTKStaticCast(TTK_ENUM_TYPE(Enum), lhs) && TTKStaticCast(TTK_ENUM_TYPE(Enum), rhs); } \
inline const Enum& operator|=(Enum& lhs, const Enum rhs) { return lhs = (lhs | rhs); } \
inline const Enum& operator&=(Enum& lhs, const Enum rhs) { return lhs = (lhs & rhs); } \
inline const Enum& operator^=(Enum& lhs, const Enum rhs) { return lhs = (lhs ^ rhs); }

#endif // TTKSUPERENUM_H
