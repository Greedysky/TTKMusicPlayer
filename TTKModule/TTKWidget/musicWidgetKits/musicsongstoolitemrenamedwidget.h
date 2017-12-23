#ifndef MUSICSONGSTOOLITEMRENAMEDWIDGET_H
#define MUSICSONGSTOOLITEMRENAMEDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

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
