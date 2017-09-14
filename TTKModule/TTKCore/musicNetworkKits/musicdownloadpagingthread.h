#ifndef MUSICDOWNLOADPAGINGTHREAD_H
#define MUSICDOWNLOADPAGINGTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicnetworkabstract.h"

/*! @brief The class to abstract download paing thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadPagingThread : public MusicNetworkAbstract
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadPagingThread(QObject *parent = 0);

    virtual ~MusicDownLoadPagingThread();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Release the network object.
     */
    virtual void deleteAll() override;
    /*!
     * Start to search data from name and type bt paging.
     */
    virtual void startToPage(int offset);

    /*!
     * Return the current raw data.
     */
    inline QVariantMap getRawData() const { return m_rawData; }
    /*!
     * Return the each page max size.
     */
    inline int getPageSize() const { return m_pageSize; }
    /*!
     * Return the page total number.
     */
    inline int getPageTotal() const { return m_pageTotal; }

protected:
    QVariantMap m_rawData;
    int m_pageSize, m_pageTotal;

};

#endif // MUSICDOWNLOADPAGINGTHREAD_H
