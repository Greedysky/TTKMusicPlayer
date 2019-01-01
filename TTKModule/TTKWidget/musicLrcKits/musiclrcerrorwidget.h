#ifndef MUSICLRCERRORWIDGET_H
#define MUSICLRCERRORWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
class MusicLrcErrorWidget;
}

/*! @brief The class of the report lrc error widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcErrorWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcErrorWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcErrorWidget(QWidget *parent = nullptr);

    virtual ~MusicLrcErrorWidget();

public Q_SLOTS:
    /*!
     * Differ radio button clicked by index.
     */
    void buttonClicked(int index);
    /*!
     * Text area text value changed.
     */
    void textAreaChanged();
    /*!
     * Confirm button clicked.
     */
    void confirmButtonClicked();
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    Ui::MusicLrcErrorWidget *m_ui;

};

#endif // MUSICLRCERRORWIDGET_H
