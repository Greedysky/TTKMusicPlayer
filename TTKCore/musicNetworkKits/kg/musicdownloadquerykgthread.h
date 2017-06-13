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
    explicit MusicDownLoadQueryKGThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startToSearch(QueryType type, const QString &text) override;
    /*!
     * Start to search data from name and type.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

protected:
    void readFromMusicMVAttribute(MusicObject::MusicSongInfomation *info,
                                  const QString &hash);
    /*!
     * Read mv tags(size\bitrate\url) from query results.
     */
    void readFromMusicMVInfoAttribute(MusicObject::MusicSongInfomation *info,
                                      const QVariantMap &key);
    /*!
     * Read mv info attribute from query results.
     */

};

#endif // MUSICDOWNLOADQUERYKGTHREAD_H
