#ifndef PINYINHELPER_H
#define PINYINHELPER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

class PinyinHelperPrivate;

/*! @brief The class of the chinese characters to pinyin.
 * @author jiangXiaoBai <2499971906@qq.com>
 */
class MUSIC_EXTRAS_EXPORT PinyinHelper
{
public:
    enum Format
    {
        WITH_TONE_MARK = 0, /*!< With tone*/
        WITH_TONE_NUMBER,   /*!< The number represents the tone*/
        WITHOUT_TONE        /*!< Without tone*/
    };

    /*!
     * Object contsructor.
     */
    PinyinHelper();

    /**
     * Converting a single Chinese character into a tone format
     *
     * @param c Needs to be converted into phonetic characters
     * @return String Pinyin
     */
    QStringList convertToPinyinArray(const QChar &c) const;

    /**
     * Convert the string into the corresponding format of the Pinyin
     *
     * @param str String that needs to be converted
     * @param separator Phonetic separator
     * @param pinyinFormat
     * @return String Pinyin
     */
    QString convertToPinyinString(const QString &s, const QString &separator, Format pinyinFormat) const;

    /**
     * Converts a string into a tone format
     *
     * @param str String that needs to be converted
     * @param separator Phonetic separator
     * @return Pinyin with tone after conversion
     */
    QString convertToPinyinString(const QString &str, const QString &separator) const;

    /**
     * A judge whether the Chinese Chinese characters
     *
     * @param c chinese characters
     * @return Judge the result, is the Chinese character returns true, otherwise returns false
     */
    bool hasMultiPinyin(char c) const;

private:
    /**
     * The phonetic alphabet is formatted into the corresponding format
     *
     * @param pinyinString Pinyin with tone
     * @param pinyinFormat
     * @return Pinyin after format conversion
     */
    QStringList formatPinyin(QString pinyinString, Format pinyinFormat) const;

    /**
     * Converting a single Chinese character into the corresponding format
     *
     * @param c Needs to be converted into phonetic characters
     * @param pinyinFormat
     * @return Pinyin of Chinese characters
     */
    QStringList convertToPinyinArray(const QChar &c, Format pinyinFormat) const;


    TTK_DECLARE_PRIVATE(PinyinHelper)

};

#endif // PINYINRESOURCE_H
