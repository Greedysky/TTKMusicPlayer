#ifndef MUSICLOCALSONGSEARCHINLINEEDIT_H
#define MUSICLOCALSONGSEARCHINLINEEDIT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
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
    explicit MusicLocalSongSearchInlineEdit(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLocalSongSearchInlineEdit();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void initWidget(QWidget *parent);
    /*!
     * Create all widget in layout.
     */
    void resizeWindow();
    /*!
     * Resize window bound by widgte resize called.
     */

protected:
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    /*!
     * Override the widget event.
     */

    MusicLocalSongSearchPopWidget *m_popWidget;

};

#endif // MUSICLOCALSONGSEARCHINLINEEDIT_H
