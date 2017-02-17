#ifndef MUSICLOCALSONGSEARCHEDIT_H
#define MUSICLOCALSONGSEARCHEDIT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLineEdit>
#include "musicglobaldefine.h"

/*! @brief The class of the local search line edit widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearchEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MusicLocalSongSearchEdit(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

Q_SIGNALS:
    void enterFinished(const QString &text);
    /*!
     * User input the enter key emit.
     */

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    /*!
     * Override the widget event.
     */

};

#endif // MUSICLOCALSONGSEARCHEDIT_H
