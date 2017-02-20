#ifndef PINYINHELPER_H
#define PINYINHELPER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
        WITH_TONE_MARK = 0, ///With tone
        WITH_TONE_NUMBER,   ///The number represents the tone
        WITHOUT_TONE        ///Without tone
    };

    PinyinHelper();
    /*!
     * Object contsructor.
     */

    QStringList convertToPinyinArray(const QChar &c);
    /**
     * Converting a single Chinese character into a tone format
     *
     * @param c Needs to be converted into phonetic characters
     * @return String Pinyin
     */

    QString convertToPinyinString(const QString &s, const QString &separator, Format pinyinFormat);
    /**
     * Convert the string into the corresponding format of the Pinyin
     *
     * @param str String that needs to be converted
     * @param separator Phonetic separator
     * @param pinyinFormat
     * @return String Pinyin
     */

    QString convertToPinyinString(const QString &str, const QString &separator);
    /**
     * Converts a string into a tone format
     *
     * @param str String that needs to be converted
     * @param separator Phonetic separator
     * @return Pinyin with tone after conversion
     */

    bool hasMultiPinyin(char c);
    /**
     * A judge whether the Chinese Chinese characters
     *
     * @param c chinese characters
     * @return Judge the result, is the Chinese character returns true, otherwise returns false
     */

private:
    QStringList formatPinyin(QString pinyinString, Format pinyinFormat);
    /**
     * The phonetic alphabet is formatted into the corresponding format
     *
     * @param pinyinString Pinyin with tone
     * @param pinyinFormat
     * @return Pinyin after format conversion
     */

    QStringList convertToPinyinArray(const QChar &c, Format pinyinFormat);
    /**
     * Converting a single Chinese character into the corresponding format
     *
     * @param c Needs to be converted into phonetic characters
     * @param pinyinFormat
     * @return Pinyin of Chinese characters
     */

    TTK_DECLARE_PRIVATE(PinyinHelper)

};

#endif // PINYINRESOURCE_H
