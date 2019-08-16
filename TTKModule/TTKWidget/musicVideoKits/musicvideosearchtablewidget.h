#ifndef MUSICVIDEOSEARCHTABLEWIDGET_H
#define MUSICVIDEOSEARCHTABLEWIDGET_H

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

#include "musicquerytablewidget.h"
#include "musicdownloadquerythreadabstract.h"

Q_DECLARE_METATYPE(MusicObject::MusicSongInformation)

/*! @brief The class of the video item play data item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_VIDEO_EXPORT MusicVideoItem
{
    QString m_name;
    QString m_url;
    QString m_id;
    QString m_server;
}MusicVideoItem;


/*! @brief The class of the video search table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_VIDEO_EXPORT MusicVideoSearchTableWidget : public MusicQueryItemTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicVideoSearchTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicVideoSearchTableWidget(QWidget *parent = nullptr);

    virtual ~MusicVideoSearchTableWidget();

    /*!
     * Start search query by text.
     */
    virtual void startSearchQuery(const QString &text) override;
    /*!
     * Start search query by given id.
     */
    void startSearchSingleQuery(const QString &text);
    /*!
     * Start search query by given data.
     */
    void startSearchSingleQuery(const QVariant &data);
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
    void mediaUrlNameChanged(const MusicVideoItem &item);

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
    virtual void createSearchedItem(const MusicSearchedItem &songItem) override;
    /*!
     * Item has double clicked.
     */
    virtual void itemDoubleClicked(int row, int column) override;
    /*!
     * Get music media information data.
     */
    void getMusicMediaInfo(MusicObject::MusicSongAttributes &data);
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
    bool m_singleRadioMode;

};

#endif // MUSICVIDEOSEARCHTABLEWIDGET_H
