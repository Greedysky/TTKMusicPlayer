#ifndef MUSICLRCFROMKRC_H
#define MUSICLRCFROMKRC_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicglobaldefine.h"

/*! @brief The class of the krc to lrc.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcFromKrc
{
public:
    MusicLrcFromKrc();
    /*!
     * Object contsructor.
     */
    ~MusicLrcFromKrc();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    bool decode(const QString &input, const QString &output = QString());
    /*!
     * Decode krc file to lrc by input file and output file.
     */
    QByteArray getDecodeString() const;
    /*!
     * Get decode string.
     */

protected:
    int sncasecmp(char *s1, char *s2, size_t n);
    /*!
     * Sncasecmp by input and size n.
     */
    int decompression(unsigned char *src, size_t srcsize, size_t *dstsize);
    /*!
     * Decompress the krc file data to normal data.
     */
    int isfilter(char *tok);
    /*!
     * Check the input string is filtered or not.
     */
    void createLrc(unsigned char *lrc, int lrclen);
    /*!
     * Create lrc by input data and length.
     */

    uchar *m_resultBytes;
    QByteArray m_data;

};

#endif // MUSICLRCFROMKRC_H
