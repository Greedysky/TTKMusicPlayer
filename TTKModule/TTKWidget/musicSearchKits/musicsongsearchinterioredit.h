#ifndef MUSICSONGSEARCHINTERIOREDIT_H
#define MUSICSONGSEARCHINTERIOREDIT_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongSearchInteriorEdit(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongSearchInteriorEdit();

    /*!
     * Create all widget in layout.
     */
    void initialize(QWidget *parent);

public Q_SLOTS:
    /*!
     * Selected text changed.
     */
    void setText(const QString &text);
    /*!
     * Input changed text changed.
     */
    void textChanged(const QString &text);
    /*!
     * Selected row text changed.
     */
    void selectedTextChanged(const QString &text, bool update);
    /*!
     * Suggest data changed.
     */
    void suggestDataChanged();
    /*!
     * Search discover list information finished.
     */
    void discoverInfoFinished(const QString &bytes);

private:
    /*!
     * Override the widget event.
     */
    virtual void keyReleaseEvent(QKeyEvent *event) override final;
    virtual void mousePressEvent(QMouseEvent *event) override final;
    virtual void focusOutEvent(QFocusEvent *event) override final;

    /*!
     * Set pop widget visible.
     */
    void setPopWidgetVisible(bool show);

    MusicSongSearchPopWidget *m_popWidget;
    MusicDiscoverListRequest *m_discoverRequest;
    MusicSongSuggestRequest *m_suggestRequest;

};

#endif // MUSICSONGSEARCHINTERIOREDIT_H
