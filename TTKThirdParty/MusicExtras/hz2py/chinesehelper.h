#ifndef CHINESEHELPER_H
#define CHINESEHELPER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "pinyinresource.h"

/**
 * Simple and traditional chinese characters class
 */
class MUSIC_EXTRAS_EXPORT ChineseHelper
{
public:
    /*!
     * Get class object name.
     */
    static QString getClassName();

    /**
     * The single traditional Chinese characters into simplified Chinese characters
     *
     * @param c Need to convert the traditional Chinese characters
     * @return After the conversion of the simplified Chinese characters
     */
    static QChar convertToSimplifiedChinese(const QChar &c);

    /**
     * The individual characters converted to traditional Chinese characters
     *
     * @param c The need to convert simplified Chinese characters
     * @return Converted to numerous fonts
     */
    static QChar convertToTraditionalChinese(const QChar &c);

    /**
     * The traditional Chinese characters into simplified Chinese characters
     *
     * @param str Need to convert the traditional Chinese characters
     * @return Converted to simplified
     */
    static QString convertToSimplifiedChinese(const QString &str);

    /**
     * Convert the simplified traditional characters
     *
     * @param str The need to convert simplified Chinese characters
     * @return Converted to numerous fonts
     */
    static QString convertToTraditionalChinese(const QString &str);

    /**
     * To determine whether a character is a traditional character
     *
     * @param c Character needed to be judged
     * @return Is the traditional character returns true, otherwise returns false
     */
    static bool isTraditionalChinese(const QChar &c);

    /**
     * To determine whether a character is a Chinese character
     *
     * @param c Character needed to be judged
     * @return Is the Chinese character returns true, otherwise returns false
     */
    static bool isChinese(const QChar &c);

    /**
     * Determine whether the string contains Chinese
     * @param str character string
     * @return Contains the Chinese characters to return to true, or return to false
     */
    static bool containsChinese(const QString &str);

private:
    ChineseHelper() {}

    static QMap<QString, QString> m_data;

};

#endif // CHINESEHELPER_H
