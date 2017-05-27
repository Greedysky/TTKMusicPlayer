#ifndef MUSICPLAYBACKMODEPOPWIDGET_H
#define MUSICPLAYBACKMODEPOPWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musictoolmenuwidget.h"

/*! @brief The class of the play back mode pop widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlayBackModePopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    explicit MusicPlayBackModePopWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void initWidget();
    /*!
     * Create all widget in layout.
     */

    void setPlaybackMode(MusicObject::SongPlayMode mode);
    /*!
     * Set current play mode.
     */

};

#endif // MUSICPLAYBACKMODEPOPWIDGET_H
