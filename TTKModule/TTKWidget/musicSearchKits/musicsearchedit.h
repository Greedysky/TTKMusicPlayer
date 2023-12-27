#ifndef MUSICSEARCHEDIT_H
#define MUSICSEARCHEDIT_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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
 ***************************************************************************/

#include <QKeyEvent>

#include "musicglobaldefine.h"
#include "musicwidgetheaders.h"

/*! @brief The class of the search line edit widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSearchEdit : public QLineEdit
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSearchEdit)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSearchEdit(QWidget *parent = nullptr);

Q_SIGNALS:
    /*!
     * User input the enter key.
     */
    void enterFinished(const QString &text);

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

};

#endif // MUSICSEARCHEDIT_H
