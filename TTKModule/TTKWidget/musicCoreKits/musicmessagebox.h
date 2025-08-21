#ifndef MUSICMESSAGEBOX_H
#define MUSICMESSAGEBOX_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicMessageBox;
}

/*! @brief The class of the messagebox widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicMessageBox : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicMessageBox(QWidget *parent = nullptr);
    explicit MusicMessageBox(const QString &text, QWidget *parent = nullptr);
    MusicMessageBox(const QString &title, const QString &text, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicMessageBox();

    /*!
     * Set box title.
     */
    void setTitle(const QString &text) const;
    /*!
     * Get box title.
     */
    QString title() const;

    /*!
     * Set context text.
     */
    void setText(const QString &text) const;
    /*!
     * Get context text.
     */
    QString text() const;

public Q_SLOTS:
    /*!
     * Confirm or cancal button clicked, set the state.
     */
    void buttonClicked(int index);

private:
    Ui::MusicMessageBox *m_ui;

};

#endif // MUSICMESSAGEBOX_H
