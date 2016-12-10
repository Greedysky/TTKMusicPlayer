#ifndef MUSICSONGSTOOLITEMRENAMEDWIDGET_H
#define MUSICSONGSTOOLITEMRENAMEDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLineEdit>
#include "musicglobaldefine.h"

/*! @brief The class of the song tool item rename widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongsToolItemRenamedWidget : public QLineEdit
{
    Q_OBJECT
public:
    MusicSongsToolItemRenamedWidget(const QString &originText, QWidget *parent = 0);
    /*!
     * Object contsructor by index and origin name.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

Q_SIGNALS:
    void renameFinished(const QString &text);
    /*!
     * Tool rename is finished it emit.
     */

public Q_SLOTS:
    void renameFinished()
    {
        emit renameFinished(text());
    }
    /*!
     * Tool rename is finished.
     */

protected:
    virtual void focusOutEvent(QFocusEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

};

#endif // MUSICSONGSTOOLITEMRENAMEDWIDGET_H
