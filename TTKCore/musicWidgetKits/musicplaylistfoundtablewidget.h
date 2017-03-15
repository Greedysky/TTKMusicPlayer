#ifndef MUSICPLAYLISTFOUNDTABLEWIDGET_H
#define MUSICPLAYLISTFOUNDTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicqueryfoundtablewidget.h"

/*! @brief The class of the playlist music found table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlaylistFoundTableWidget : public MusicQueryFoundTableWidget
{
    Q_OBJECT
public:
    explicit MusicPlaylistFoundTableWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicPlaylistFoundTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setQueryInput(MusicDownLoadQueryThreadAbstract *query);
    /*!
     * Set network query input.
     */
    void setConnectObject(QObject *obj);
    /*!
     * Set connect object.
     */

};

#endif // MUSICPLAYLISTFOUNDTABLEWIDGET_H
