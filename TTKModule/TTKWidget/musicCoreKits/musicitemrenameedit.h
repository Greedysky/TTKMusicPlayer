#ifndef MUSICITEMRENAMEEIDT_H
#define MUSICITEMRENAMEEIDT_H

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

#include <QLineEdit>
#include "musicglobaldefine.h"

/*! @brief The class of the item rename edit.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicItemRenameEidt : public QLineEdit
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicItemRenameEidt)
public:
    /*!
     * Object constructor by index and origin name.
     */
    explicit MusicItemRenameEidt(QWidget *parent = nullptr);
    explicit MusicItemRenameEidt(const QString &text, QWidget *parent = nullptr);

Q_SIGNALS:
    /*!
     * Tool rename is finished it.
     */
    void renameFinished(const QString &text);

public Q_SLOTS:
    /*!
     * Input text data changed.
     */
    void textChanged(const QString &text);
    /*!
     * Tool rename is finished.
     */
    void renameFinished();
    /*!
     * Animation close changed.
     */
    void animationCloseChanged();

private:
    /*!
     * Override the widget event.
     */
    virtual void focusOutEvent(QFocusEvent *event) override final;

    bool m_focusBlock;

};

#endif // MUSICITEMRENAMEEIDT_H
