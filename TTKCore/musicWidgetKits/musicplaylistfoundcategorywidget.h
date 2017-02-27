#ifndef MUSICPLAYLISTFOUNDCATEGORYWIDGET_H
#define MUSICPLAYLISTFOUNDCATEGORYWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musictoolmenuwidget.h"

/*! @brief The class of the playlist music found category widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlaylistFoundCategoryWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    explicit MusicPlaylistFoundCategoryWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicPlaylistFoundCategoryWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    void popupMenu();
    /*!
     * To popup menu.
     */

protected:
    void initWidget();
    /*!
     * Create all widget in layout.
     */

};

#endif // MUSICPLAYLISTFOUNDCATEGORYWIDGET_H
