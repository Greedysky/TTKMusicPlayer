#ifndef MUSICVIDEOTABLEWIDGET_H
#define MUSICVIDEOTABLEWIDGET_H

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

#include "musicquerytablewidget.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class of the video search table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_VIDEO_EXPORT MusicVideoTableWidget : public MusicQueryItemTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicVideoTableWidget(QWidget *parent = 0);

    virtual ~MusicVideoTableWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start search query by text.
     */
    virtual void startSearchQuery(const QString &text) override;
    /*!
     * Data download to local file.
     */
    virtual void musicDownloadLocal(int row) override;
    /*!
     * Resize widget size by delta.
     */
    void resizeWindow(int delta);

Q_SIGNALS:
    /*!
     * Set current media name and url to play.
     */
    void mvURLNameChanged(const QString &name, const QString &data);

public Q_SLOTS:
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellEntered(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Clear All Items.
     */
    virtual void clearAllItems() override;
    /*!
     * Create searched items.
     */
    virtual void createSearchedItems(const MusicSearchedItem &songItem) override;
    /*!
     * Item has double clicked.
     */
    virtual void itemDoubleClicked(int row, int column) override;
    /*!
     * Get music mv information data.
     */
    void getMusicMvInfo(MusicObject::MusicSongAttributes &data);
    /*!
     * Download current mv by controller.
     */
    void downloadLocalFromControl();

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Download movie to local file by index.
     */
    void downloadLocalMovie(int row);

    QString m_currentSongName;

};

#endif // MUSICVIDEOTABLEWIDGET_H
