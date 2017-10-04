#ifndef MUSICDOWNLOADQUERYYYTTHREAD_H
#define MUSICDOWNLOADQUERYYYTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to yinyuetai query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryYYTThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadQueryYYTThread(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start to search data from name and type.
     */
    virtual void startToSearch(QueryType type, const QString &text) override;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

protected:
    /*!
     * Read mv tags(size\bitrate\url) from query results.
     */
    void readFromMusicMVAttribute(MusicObject::MusicSongInformation *info,
                                  const QString &id);
    /*!
     * Read mv info attribute from query results.
     */
    void readFromMusicMVInfoAttribute(MusicObject::MusicSongInformation *info, const QString &url,
                                      const QString &size, const QString &duration);
    /*!
     * Find time string by attrs.
     */
    QString findTimeStringByAttrs(const MusicObject::MusicSongAttributes &attrs);

};

#endif // MUSICDOWNLOADQUERYYYTTHREAD_H
