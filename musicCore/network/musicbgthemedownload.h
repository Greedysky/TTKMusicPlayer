#ifndef MUSICBGTHEMESLIDE_H
#define MUSICBGTHEMESLIDE_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicobject.h"
#include "musiclibexportglobal.h"

class MUSIC_NETWORK_EXPORT MusicBgThemeDownload : public QObject
{
    Q_OBJECT
public:
    explicit MusicBgThemeDownload(const QString &name, const QString &save,
                                  QObject *parent = 0);
    ~MusicBgThemeDownload();

signals:
    void musicBgDownloadFinished();

public slots:
    void downLoadFinished(const QString &name);
    void bgDownLoadFinished(const QString &name);

protected:
    QString m_artName;
    QString m_savePath;
    int m_index;
    int m_counter;

};

#endif // MUSICBGTHEMESLIDE_H
