#ifndef MUSICSONGSLISTITEMINFOWIDGET_H
#define MUSICSONGSLISTITEMINFOWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsong.h"
#include "musicglobaldefine.h"
#include "musicabstractmovewidget.h"

namespace Ui {
class MusicSongsListItemInfoWidget;
}

/*! @brief The class of the song list item info widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongsListItemInfoWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongsListItemInfoWidget(QWidget *parent = 0);

    ~MusicSongsListItemInfoWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set music song information.
     */
    void setMusicSongInformation(const MusicSong &info);

protected:
    /*!
     * Show artist small picture, if no exsit there is default pic.
     */
    bool showArtPicture(const QString &name);

    Ui::MusicSongsListItemInfoWidget *m_ui;

};

#endif // MUSICSONGSLISTITEMINFOWIDGET_H
