#ifndef MUSICLOCALSONGSEARCHINLINEEDIT_H
#define MUSICLOCALSONGSEARCHINLINEEDIT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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

#include "musiclocalsongsearchedit.h"

class MusicDownLoadDiscoverListThread;
class MusicLocalSongSearchPopWidget;
class MusicDownLoadQueryThreadAbstract;

/*! @brief The class of the net search line edit widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_SEARCH_EXPORT MusicLocalSongSearchInlineEdit : public MusicLocalSongSearchEdit
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLocalSongSearchInlineEdit(QWidget *parent = 0);

    virtual ~MusicLocalSongSearchInlineEdit();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Create all widget in layout.
     */
    void initWidget(QWidget *parent);

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
    void searchTopListInfoFinished(const QString &data);

protected:
    /*!
     * Pop widget changed.
     */
    void popWidgetChanged(const QString &text);
    /*!
     * Override the widget event.
     */
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    MusicLocalSongSearchPopWidget *m_popWidget;
    MusicDownLoadDiscoverListThread *m_discoverThread;
    MusicDownLoadQueryThreadAbstract *m_suggestThread;

};

#endif // MUSICLOCALSONGSEARCHINLINEEDIT_H
