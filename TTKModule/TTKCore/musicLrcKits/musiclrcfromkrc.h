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
    /*!
     * Object contsructor.
     */
    MusicLrcFromKrc();

    ~MusicLrcFromKrc();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Decode krc file to lrc by input file and output file.
     */
    bool decode(const QString &input, const QString &output = QString());
    /*!
     * Get decode string.
     */
    QByteArray getDecodeString() const;

protected:
    /*!
     * Sncasecmp by input and size n.
     */
    int sncasecmp(char *s1, char *s2, size_t n);
    /*!
     * Decompress the krc file data to normal data.
     */
    int decompression(unsigned char *src, size_t srcsize, size_t *dstsize);
    /*!
     * Check the input string is filtered or not.
     */
    int isfilter(char *tok);
    /*!
     * Create lrc by input data and length.
     */
    void createLrc(unsigned char *lrc, int lrclen);

    uchar *m_resultBytes;
    QByteArray m_data;

};

#endif // MUSICLRCFROMKRC_H
