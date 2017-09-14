#ifndef MUSICDOWNLOADQUERYKGTHREAD_H
#define MUSICDOWNLOADQUERYKGTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadkginterface.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to kugou query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryKGThread : public MusicDownLoadQueryThreadAbstract,
                                                        private MusicDownLoadKGInterface
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadQueryKGThread(QObject *parent = 0);

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
                                  const QString &hash);
    /*!
     * Read mv info attribute from query results.
     */
    void readFromMusicMVInfoAttribute(MusicObject::MusicSongInformation *info,
                                      const QVariantMap &key);

};

#endif // MUSICDOWNLOADQUERYKGTHREAD_H
