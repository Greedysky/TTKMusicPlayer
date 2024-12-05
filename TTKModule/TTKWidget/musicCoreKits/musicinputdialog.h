#ifndef MUSICINPUTDIALOG_H
#define MUSICINPUTDIALOG_H

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
class MusicInputDialog;
}

/*! @brief The class of the input widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicInputDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicInputDialog)
public:
    /*!
     * Object constructor.
     */
    explicit MusicInputDialog(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicInputDialog();

    /*!
     * Get the text that the user input.
     */
    QString text() const;
    /*!
     * Set placeholder text.
     */
    void setPlaceholderText(const QString &text);

private:
    Ui::MusicInputDialog *m_ui;

};

#endif // MUSICINPUTDIALOG_H
