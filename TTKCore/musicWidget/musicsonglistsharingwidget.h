#ifndef MUSICSONGLISTSHARINGWIDGET_H
#define MUSICSONGLISTSHARINGWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicSongListSharingWidget;
}

/*! @brief The class of the song list share widget.
 * @author Greedysky <greedysky@163.com>
 */

class MUSIC_WIDGET_EXPORT MusicSongListSharingWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicSongListSharingWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongListSharingWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

public Q_SLOTS:
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    Ui::MusicSongListSharingWidget *m_ui;

};

#endif // MUSICSONGLISTSHARINGWIDGET_H
