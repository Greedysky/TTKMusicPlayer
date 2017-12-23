#ifndef CHINESEHELPER_H
#define CHINESEHELPER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

#include "pinyinresource.h"

class ChineseHelperPrivate;

/*! @brief The class of the simple and traditional chinese characters.
 * @author jiangXiaoBai <2499971906@qq.com>
 */
class MUSIC_EXTRAS_EXPORT ChineseHelper
{
public:
    /*!
     * Object contsructor.
     */
    ChineseHelper();

    /**
     * The single traditional Chinese characters into simplified Chinese characters
     *
     * @param c Need to convert the traditional Chinese characters
     * @return After the conversion of the simplified Chinese characters
     */
    QChar convertToSimplifiedChinese(const QChar &c);

    /**
     * The individual characters converted to traditional Chinese characters
     *
     * @param c The need to convert simplified Chinese characters
     * @return Converted to numerous fonts
     */
    QChar convertToTraditionalChinese(const QChar &c);

    /**
     * The traditional Chinese characters into simplified Chinese characters
     *
     * @param str Need to convert the traditional Chinese characters
     * @return Converted to simplified
     */
    QString convertToSimplifiedChinese(const QString &str);

    /**
     * Convert the simplified traditional characters
     *
     * @param str The need to convert simplified Chinese characters
     * @return Converted to numerous fonts
     */
    QString convertToTraditionalChinese(const QString &str);

    /**
     * To determine whether a character is a traditional character
     *
     * @param c Character needed to be judged
     * @return Is the traditional character returns true, otherwise returns false
     */
    bool isTraditionalChinese(const QChar &c);

    /**
     * To determine whether a character is a Chinese character
     *
     * @param c Character needed to be judged
     * @return Is the Chinese character returns true, otherwise returns false
     */
    bool isChinese(const QChar &c);

    /**
     * Determine whether the string contains Chinese
     * @param str character string
     * @return Contains the Chinese characters to return to true, or return to false
     */
    bool containsChinese(const QString &str);

private:
    MUSIC_DECLARE_PRIVATE(ChineseHelper)

};

#endif // CHINESEHELPER_H
