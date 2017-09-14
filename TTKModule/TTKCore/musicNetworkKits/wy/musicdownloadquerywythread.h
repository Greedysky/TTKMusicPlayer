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
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadQueryWYThread(QObject *parent = 0);

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
     * Start to search mv list by id.
     */
    void startMVListQuery(int id);

};

#endif // MUSICDOWNLOADQUERYWYTHREAD_H
