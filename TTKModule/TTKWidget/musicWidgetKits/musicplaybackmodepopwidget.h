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
    /*!
     * Object contsructor.
     */
    explicit MusicPlayBackModePopWidget(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Create all widget in layout.
     */
    void initWidget();

    /*!
     * Set current play mode.
     */
    void setPlaybackMode(MusicObject::PlayMode mode);

};

#endif // MUSICPLAYBACKMODEPOPWIDGET_H
