#ifndef MUSICLOCALSONGSEARCHEDIT_H
#define MUSICLOCALSONGSEARCHEDIT_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLineEdit>
#include "musiclibexportglobal.h"

class MUSIC_SEARCH_EXPORT MusicLocalSongSearchEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchEdit(QWidget *parent = 0);

protected:
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

};

#endif // MUSICLOCALSONGSEARCHEDIT_H
