#ifndef MUSICLOCALSONGSEARCHDIALOG_H
#define MUSICLOCALSONGSEARCHDIALOG_H

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

#include "musicglobaldefine.h"
#include "musicwidgetheaders.h"

class MusicLocalSongSearchEdit;

/*! @brief The class of the local song search widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearchDialog : public QDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLocalSongSearchDialog)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongSearchDialog(QWidget *parent = nullptr);

    ~MusicLocalSongSearchDialog();

    /*!
     * Get the search text that the user searched.
     */
    QString getSearchedText() const;
    /*!
     * Clear the search text.
     */
    void clearSearchedText() const;

public Q_SLOTS:
    /*!
     * Override the close function.
     */
    bool close();

private:
    MusicLocalSongSearchEdit *m_searchLine;

};

#endif // MUSICLOCALSONGSEARCHDIALOG_H
