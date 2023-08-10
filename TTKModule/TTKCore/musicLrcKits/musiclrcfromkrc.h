#ifndef MUSICLRCFROMKRC_H
#define MUSICLRCFROMKRC_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

/*! @brief The class of the krc to lrc.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcFromKrc
{
    TTK_DECLARE_MODULE(MusicLrcFromKrc)
public:
    /*!
     * Object constructor.
     */
    MusicLrcFromKrc();
    /*!
     * Object destructor.
     */
    ~MusicLrcFromKrc();

    /*!
     * Decode krc file to lrc by input file and output file.
     */
    bool decode(const QString &input, const QString &output = QString());
    /*!
     * Get decode string.
     */
    QByteArray decodeString() const;

private:
    /*!
     * Sncasecmp by input and size n.
     */
    int sncasecmp(char *s1, char *s2, size_t n);
    /*!
     * Decompress the krc file data to normal data.
     */
    int decompression(uchar *src, size_t srcsize, size_t *dstsize);
    /*!
     * Check the input string is filtered or not.
     */
    int isfilter(char *tok);
    /*!
     * Create lrc by input data and length.
     */
    void createLrc(uchar *lrc, int lrclen);

    uchar *m_resultBytes;
    QByteArray m_data;

};

#endif // MUSICLRCFROMKRC_H
