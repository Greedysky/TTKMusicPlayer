#ifndef MUSICLOCALSONGSEARCHINLINEEDIT_H
#define MUSICLOCALSONGSEARCHINLINEEDIT_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclocalsongsearchedit.h"

class MusicLocalSongSearchPopWidget;

class MUSIC_SEARCH_EXPORT MusicLocalSongSearchInlineEdit : public MusicLocalSongSearchEdit
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchInlineEdit(QWidget *parent = 0);
    virtual ~MusicLocalSongSearchInlineEdit();

protected:
    virtual void focusOutEvent(QFocusEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    MusicLocalSongSearchPopWidget *m_popWidget;

};

#endif // MUSICLOCALSONGSEARCHINLINEEDIT_H
