#ifndef TTKREGULAREXPRESSION_H
#define TTKREGULAREXPRESSION_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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
#if TTK_QT_VERSION_CHECK(5,0,0)
#  include <QRegularExpression>
#else
#  include <QRegExp>
#endif

/*! @brief The class of the regular expression.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKRegularExpression
{
public:
    /*!
     * Object constructor.
     */
    TTKRegularExpression() = default;
    explicit TTKRegularExpression(const QString &pattern);

    /*!
     * Returns the pattern string of the regular expression.
     */
    QString pattern() const;
    /*!
     * Sets the pattern string of the regular expression to pattern.
     */
    void setPattern(const QString &v);

    /*!
     * Returns true if the regular expression matched against the subject string, or false otherwise.
     */
    bool hasMatch(const QString &str);

    /*!
     * Searches the byte array ba, from byte position from (default 0, i.e. from the first byte).
     */
    int match(const QString &str, int pos = 0);

    /*!
     * Returns the substring captured by the nth capturing group.
     */
    QString captured(int index) const;

    /*!
     * Returns the length of the substring captured by the nth capturing group.
     */
    int capturedLength() const;

    /*!
     * Returns the greediness option.
     */
    bool greedinessOption() const;

    /*!
     * Set the greediness option.
     */
    void setGreedinessOption(bool v);

    /*!
     * Escapes all characters of string.
     */
    static QString escape(const QString &str);

#if TTK_QT_VERSION_CHECK(5,0,0)
    operator QRegularExpression &();
    operator const QRegularExpression &() const;
private:
    QRegularExpression m_regular;
    QRegularExpressionMatch m_match;
#else
    operator QRegExp &();
    operator const QRegExp &() const;
private:
    QRegExp m_regular;
#endif

};

#endif // TTKREGULAREXPRESSION_H
