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
    /*!
     * Object contsructor by index and origin name.
     */
    explicit MusicSongsToolItemRenamedWidget(const QString &originText, QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

Q_SIGNALS:
    /*!
     * Tool rename is finished it emit.
     */
    void renameFinished(const QString &text);

public Q_SLOTS:
    /*!
     * Tool rename is finished.
     */
    void renameFinished()
    {
        emit renameFinished(text());
    }

protected:
    /*!
     * Override the widget event.
     */
    virtual void focusOutEvent(QFocusEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

};

#endif // MUSICSONGSTOOLITEMRENAMEDWIDGET_H
