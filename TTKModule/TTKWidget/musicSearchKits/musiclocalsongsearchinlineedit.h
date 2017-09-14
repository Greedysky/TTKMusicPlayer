#ifndef MUSICLOCALSONGSEARCHINLINEEDIT_H
#define MUSICLOCALSONGSEARCHINLINEEDIT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclocalsongsearchedit.h"

class MusicLocalSongSearchPopWidget;

/*! @brief The class of the net search line edit widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearchInlineEdit : public MusicLocalSongSearchEdit
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongSearchInlineEdit(QWidget *parent = 0);

    virtual ~MusicLocalSongSearchInlineEdit();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Create all widget in layout.
     */
    void initWidget(QWidget *parent);

protected:
    /*!
     * Override the widget event.
     */
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    MusicLocalSongSearchPopWidget *m_popWidget;

};

#endif // MUSICLOCALSONGSEARCHINLINEEDIT_H
