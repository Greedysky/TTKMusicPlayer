#ifndef MUSICSONGSEARCHINTERIOREDIT_H
#define MUSICSONGSEARCHINTERIOREDIT_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#include "musicsearchedit.h"

class MusicDiscoverListRequest;
class MusicSongSearchPopWidget;
class MusicSongSuggestRequest;

/*! @brief The class of the net search interior edit widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongSearchInteriorEdit : public MusicSearchEdit
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongSearchInteriorEdit)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongSearchInteriorEdit(QWidget *parent = nullptr);
    ~MusicSongSearchInteriorEdit();

    /*!
     * Create all widget in layout.
     */
    void initialize(QWidget *parent);

    /*!
     * Close pop widget possible.
     */
    void closePopWidget();

public Q_SLOTS:
    /*!
     * Input changed text changed.
     */
    void textChanged(const QString &text);
    /*!
     * Suggest data changed.
     */
    void suggestDataChanged();
    /*!
     * Search top list information finished.
     */
    void searchToplistInfoFinished(const QString &bytes);

protected:
    /*!
     * Pop widget changed.
     */
    void popWidgetChanged(const QString &text);
    /*!
     * Override the widget event.
     */
    virtual void focusInEvent(QFocusEvent *event) override final;
    virtual void leaveEvent(QEvent *event) override final;

    MusicSongSearchPopWidget *m_popWidget;
    MusicDiscoverListRequest *m_discoverRequest;
    MusicSongSuggestRequest *m_suggestRequest;

};

#endif // MUSICSONGSEARCHINTERIOREDIT_H
