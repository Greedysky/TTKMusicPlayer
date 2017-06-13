#ifndef MUSICDOWNLOADQUERYWYTHREAD_H
#define MUSICDOWNLOADQUERYWYTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadwyinterface.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to wangyi query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryWYThread : public MusicDownLoadQueryThreadAbstract,
                                                        private MusicDownLoadWYInterface
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryWYThread(QObject *parent = 0);
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
    void startMVListQuery(int id);
    /*!
     * Start to search mv list by id.
     */

};

#endif // MUSICDOWNLOADQUERYWYTHREAD_H
