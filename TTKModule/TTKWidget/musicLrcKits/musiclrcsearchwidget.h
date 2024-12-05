#ifndef MUSICLRCSEARCHWIDGET_H
#define MUSICLRCSEARCHWIDGET_H

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
class MusicLrcSearchWidget;
}

/*! @brief The class of the lrc search widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcSearchWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcSearchWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcSearchWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLrcSearchWidget();

public Q_SLOTS:
    /*!
     * Set lrc search finished.
     */
    void lrcSearchFinished() const;
    /*!
     * Lrc search button clicked.
     */
    void lrcSearchButtonClicked() const;
    /*!
     * Lrc search download clicked.
     */
    void lrcSearchDownloadClicked();
    /*!
     * Lrc download state is finished.
     */
    void lrcDownloadStateChanged(const QString &string);
    /*!
     * Set current song name.
     */
    void setCurrentSongName(const QString &name) const;

private:
    Ui::MusicLrcSearchWidget *m_ui;

};

#endif // MUSICLRCSEARCHWIDGET_H
