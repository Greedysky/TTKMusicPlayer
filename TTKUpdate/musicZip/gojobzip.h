#ifndef GOJOBZIP_H
#define GOJOBZIP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QStringList>
#include "musicupdateglobaldefine.h"

#include "quazip.h"
#include "quazipfile.h"

#include <stdio.h>

#ifdef STDC
#  include <string.h>
#  include <stdlib.h>
#endif

#ifdef USE_MMAP
#  include <sys/types.h>
#  include <sys/mman.h>
#  include <sys/stat.h>
#endif

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#ifdef VMS
#  define unlink delete
#  define GZ_SUFFIX "-gz"
#endif
#ifdef RISCOS
#  define unlink remove
#  define GZ_SUFFIX "-gz"
#  define fileno(file) file->__file
#endif
#if defined(__MWERKS__) && __dest_os != __be_os && __dest_os != __win32_os
#  include <unix.h> /* for fileno */
#endif


#ifndef GZ_SUFFIX
#  define GZ_SUFFIX ".gz"
#endif
#define SUFFIX_LEN (sizeof(GZ_SUFFIX)-1)

#define BUFLEN      16384
#define MAX_NAME_LEN 1024

#ifdef MAXSEG_64K
#  define local static
   /* Needed for systems with limitation on stack size. */
#else
#  define local
#endif

class MUSIC_UPDATE_EXPORT PackZip : public QObject
{
    Q_OBJECT
public:
    explicit PackZip(QObject *parent = 0);
    ~PackZip();

    static QString getClassName();
    void setJob(const QString &packfolder, const QString &zipoutfolder);
    QString biteorMega(int peso);
    bool makeGzipOneFile(QString filelocation);

    bool isFile(const QString &fullFileName);
    void gzCompress(FILE *in, gzFile out);
    bool unZipTo(const QString &zipfile, const QString &outputdir);

Q_SIGNALS:
    void process(float percent, const QString &file);
    void zipEnd(const QString &result);
    void zipError();
    void status(int state);

private:
    void init();
    void resetDir();
    void indexDir(const QString &dir);

    bool canmake;
    QStringList dirandfile;
    QString startnow, startdir, zipdir, zipfile, selfname;

};


#endif // GOJOBZIP_H

