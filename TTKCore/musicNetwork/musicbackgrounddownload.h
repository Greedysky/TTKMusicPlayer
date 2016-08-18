#ifndef MUSICBACKGROUNDDOWNLOAD_H
#define MUSICBACKGROUNDDOWNLOAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of downloading art background image.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicBackgroundDownload : public QObject
{
    Q_OBJECT
public:
    MusicBackgroundDownload(const QString &name, const QString &save,
                         QObject *parent = 0);
    /*!
     * Object contsructor provide artist name and save local path.
     */
    ~MusicBackgroundDownload();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void startToDownload();
    /*!
     * Start to download artist picture from net.
     */

Q_SIGNALS:
    void musicBgDownloadFinished();
    /*!
     * Artist picture to download finished.
     */

public Q_SLOTS:
    void downLoadFinished(const QByteArray &data);
    /*!
     * Download data from kuwo net finished.
     */
    void bgDownLoadFinished();
    /*!
     * Download artist picture from kuwo net finished.
     */

protected:
    QString m_artName;
    QString m_savePath;
    int m_index;
    int m_counter;

};

#endif // MUSICBACKGROUNDDOWNLOAD_H
