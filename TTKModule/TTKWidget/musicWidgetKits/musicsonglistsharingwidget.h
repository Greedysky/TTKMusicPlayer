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
    /*!
     * Object contsructor.
     */
    explicit MusicSongListSharingWidget(QWidget *parent = 0);

    virtual ~MusicSongListSharingWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

public Q_SLOTS:
    /*!
     * Override exec function.
     */
    virtual int exec();
    /*!
     * Switch to diff widget.
     */
    void switchToDiffWidget(int index);
    /*!
     * Write copy button clicked.
     */
    void writeMainCopyButtonClicked();
    /*!
     * Read button clicked.
     */
    void readMainButtonClicked();

protected:
    /*!
     * Create write key.
     */
    void createWriteKey();

    Ui::MusicSongListSharingWidget *m_ui;

};

#endif // MUSICSONGLISTSHARINGWIDGET_H
