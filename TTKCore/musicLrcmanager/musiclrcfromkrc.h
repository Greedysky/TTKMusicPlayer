#ifndef MUSICLRCFROMKRC_H
#define MUSICLRCFROMKRC_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicglobaldefine.h"

/*! @brief The class of the krc 2 lrc.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcFromKrc
{
public:
    MusicLrcFromKrc();
    ~MusicLrcFromKrc();

    bool decode(const QString &input, const QString &output = QString());
    QByteArray getDecodeString() const;

protected:
    int sncasecmp(char *s1, char *s2, size_t n);
    int decompression(unsigned char *src, size_t srcsize, size_t *dstsize);
    int isfilter(char *tok);
    void createLrc(unsigned char *lrc, size_t lrclen);

    uchar *m_resultBytes;
    QByteArray m_data;

};

#endif // MUSICLRCFROMKRC_H
