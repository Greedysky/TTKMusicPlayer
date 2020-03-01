#ifndef MUSICMESSAGEFEEDBACKDIALOG_H
#define MUSICMESSAGEFEEDBACKDIALOG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#include "musicabstractmovedialog.h"

namespace Ui {
    class MusicMessageFeedbackDialog;
}
/*! @brief The class of the message feedback dialog.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicMessageFeedbackDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicMessageFeedbackDialog)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicMessageFeedbackDialog(QWidget *parent = nullptr);

    virtual ~MusicMessageFeedbackDialog();

public Q_SLOTS:
    /*!
     * Override exec function.
     */
    virtual int exec();
    /*!
     * Open file button clicked.
     */
    void openFileButtonClicked();
    /*!
     * Confirm button clicked.
     */
    void confirmButtonClicked();

protected:
    Ui::MusicMessageFeedbackDialog *m_ui;

    QStringList m_path;

};

#endif // MUSICMESSAGEFEEDBACKDIALOG_H
