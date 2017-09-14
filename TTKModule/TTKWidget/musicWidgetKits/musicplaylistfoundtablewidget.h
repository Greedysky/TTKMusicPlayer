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
    /*!
     * Object contsructor.
     */
    explicit MusicPlaylistFoundTableWidget(QWidget *parent = 0);

    virtual ~MusicPlaylistFoundTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set network query input.
     */
    void setQueryInput(MusicDownLoadQueryThreadAbstract *query);
    /*!
     * Set connect object.
     */
    void setConnectObject(QObject *obj);

};

#endif // MUSICPLAYLISTFOUNDTABLEWIDGET_H
